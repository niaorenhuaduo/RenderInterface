#include "..\Include\Font.h"

#include "..\Include\IRenderSystem.h"
#include "..\Include\ServiceLocator.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H
#include FT_BITMAP_H

#pragma comment(lib, "freetype.lib")


namespace ri {


std::string Font::mFontFilesDirectory = "..\\..\\Data\\Demos\\Fonts\\";


Font::Font() : mLibrary(NULL), mFace(NULL), mFontData(), mFontName("") {
}

Font::~Font() 
{
	Destroy();
}

void Font::Destroy()
{
	// Release all textures
	PageMap::iterator it = mFontData.begin();
	while(it != mFontData.end()) {
		if(!it->second.texture.IsNull()) ServiceLocator::TexMgr().ReleaseTexture(it->second.texture);

		++it;
	}

	if(mFace) FT_Done_Face(static_cast<FT_Face>(mFace));
	if(mLibrary) FT_Done_FreeType(static_cast<FT_Library>(mLibrary));

	mLibrary = NULL;
	mFace = NULL;
	mFontData.clear();
	mFontName.clear();
}

bool Font::LoadFromFile( const std::string &filename )
{
	// Start from scratch
	Destroy();

	std::string filepath = mFontFilesDirectory + filename;

	// Initialize freetype
	FT_Library library;
	if(FT_Init_FreeType(&library) != 0) {
		MessageBox(NULL, "Failed to initialize Freetype!", "error", MB_OK);
		return false;
	}

	// Initialize the face. "Arial" is an example of a face
	FT_Face face;
	if(FT_New_Face(library, filepath.c_str(), 0, &face) != 0) {
		std::string msg = "Failed to initialize the font: " + filepath;
		MessageBox(NULL, msg.c_str(), "error", MB_OK);
		FT_Done_FreeType(library);
		return false;
	}

	// Select the charmap for uniceode
	if(FT_Select_Charmap(face, FT_ENCODING_UNICODE) != 0) {
		MessageBox(NULL, "Freetype Error: Failed to select the charmap for the font!", "error", MB_OK);
		FT_Done_Face(face);
		FT_Done_FreeType(library);
		return false;
	}

	// everything works, store the pointers 
	mFace	 = face;
	mLibrary = library;

	// name of the font
	mFontName = face->family_name ? face->family_name : std::string();

	return true;
}




const Glyph& Font::GetGlyph( U32 unicode, U32 fontSize, bool bold ) const {
	assert(mLibrary && mFace && "Font::GetGlyph: Font not initialized!");
	assert(fontSize != 0 && "Font::GetGlyph: FontSize is 0!");

	// get the correct glyph table
	GlyphMap &glyphs = mFontData[fontSize].glyphs;

	// combine unicode and bold for the final index
	U32 index = ((bold ? 1 : 0) << 31) | unicode;


	GlyphMap::const_iterator it = glyphs.find(index);
	if(it != glyphs.end()) {
		return it->second;
	} else {
		Glyph glyph = LoadGlyph(unicode, fontSize, bold);
		return glyphs.insert(std::make_pair(index, glyph)).first->second;
	}
}

ri::HTexture Font::GetTexture( U32 fontSize ) const {
	return mFontData[fontSize].texture;
}

U32 Font::GetTextureSize( U32 fontSize ) const {
	return mFontData[fontSize].textureImage.GetWidth();
}

float Font::GetYOffset( U32 fontSize ) const {
	FT_Face face = static_cast<FT_Face>(mFace);

	if (face && SetCurrentSize(fontSize)) {
		return static_cast<float>(face->size->metrics.height) / static_cast<float>(1 << 6);
	} else {
		return 0.f;
	}
}

float Font::GetUnderlineOffset( U32 fontSize ) const
{
	FT_Face face = static_cast<FT_Face>(mFace);

	if (face && SetCurrentSize(fontSize))
	{
		// fixed offset for bitmap fonts
		if (!FT_IS_SCALABLE(face))
			return fontSize / 10.f;

		return -static_cast<float>(FT_MulFix(face->underline_position, face->size->metrics.y_scale)) / static_cast<float>(1 << 6);
	}
	else
	{
		return 0.f;
	}
}

float Font::GetUnderlineThickness( U32 fontSize ) const
{
	FT_Face face = static_cast<FT_Face>(mFace);

	if (face && SetCurrentSize(fontSize))
	{
		// fixed thickness for bitmap fonts
		if (!FT_IS_SCALABLE(face))
			return fontSize / 14.f;

		return static_cast<float>(FT_MulFix(face->underline_thickness, face->size->metrics.y_scale)) / static_cast<float>(1 << 6);
	}
	else
	{
		return 0.f;
	}
}


Glyph Font::LoadGlyph( U32 unicode, U32 fontSize, bool bold ) const {
	Glyph glyph;

	// Get the right face pointer
	FT_Face face = static_cast<FT_Face>(mFace);
	if(!face) return glyph;

	// set the right font size
	if(!SetCurrentSize(fontSize)) return glyph;

	// Load the character into the slot
	if(FT_Load_Char(face, unicode, FT_LOAD_TARGET_NORMAL | FT_LOAD_FORCE_AUTOHINT) != 0) return glyph;

	// Retrieve the glyph 
	FT_Glyph glyphDesc;
	if(FT_Get_Glyph(face->glyph, &glyphDesc) != 0) return glyph;

	// Apply bold
	FT_Pos weight = 1 << 6;
	bool outline = (glyphDesc->format == FT_GLYPH_FORMAT_OUTLINE);
	if(bold && outline) {
		FT_OutlineGlyph outlineglyph = reinterpret_cast<FT_OutlineGlyph>(glyphDesc);
		FT_Outline_Embolden(&outlineglyph->outline, weight);
	}

	// rasterize the glyph
	FT_Glyph_To_Bitmap(&glyphDesc, FT_RENDER_MODE_NORMAL, 0, 1);
	FT_Bitmap &bitmap = reinterpret_cast<FT_BitmapGlyph>(glyphDesc)->bitmap;

	// apply bold to bitmap glyph
	if(bold && !outline) {
		FT_Bitmap_Embolden(static_cast<FT_Library>(mLibrary), &bitmap, weight, weight);
	}

	float metricToPixels = static_cast<float>(1 << 6);
	glyph.AdvanceX = static_cast<float>(face->glyph->metrics.horiAdvance) / metricToPixels;
	if(bold) glyph.AdvanceX += static_cast<float>(weight) / metricToPixels;

	U32 height = bitmap.rows;
	U32 width  = bitmap.width;

	if((height > 0) && (width > 0)) {
		
		// leave an alpha padding around characters to remove filter errors
		const U32 padding = 1;

		Page &page = mFontData[fontSize];

		// Compute the texture coordinates
		glyph.TexCoords = FindFittingTextureRect(page, width + padding * 2, height + padding * 2);
		glyph.TexCoords.left	+= padding;
		glyph.TexCoords.top		+= padding;
		glyph.TexCoords.width	-= 2 * padding;
		glyph.TexCoords.height	-= 2 * padding;

		// Compute the bounding box
		glyph.BoundingBox.left   = static_cast<float>(face->glyph->metrics.horiBearingX) / static_cast<float>(1 << 6);
		glyph.BoundingBox.top    = -static_cast<float>(face->glyph->metrics.horiBearingY) / static_cast<float>(1 << 6);
		glyph.BoundingBox.width  = static_cast<float>(face->glyph->metrics.width) / static_cast<float>(1 << 6);
		glyph.BoundingBox.height = static_cast<float>(face->glyph->metrics.height) / static_cast<float>(1 << 6);

		// Update the font texture with the new glyph
		Image &texImage = page.textureImage;
		const U8 *pixels = bitmap.buffer;

		U32 xOffset = glyph.TexCoords.left;
		U32 yOffset = glyph.TexCoords.top;
		if(bitmap.pixel_mode == FT_PIXEL_MODE_MONO) {
			// pixels are 1 bit monochrome values
			for(U32 y=0; y<height; ++y) {
				for(U32 x=0; x<width; ++x) {

					U8 alpha = ((pixels[x / 8]) & (1 << (7 - (x % 8)))) ? 255 : 0;
					texImage.SetPixel(xOffset + x, yOffset + y, Color(255, 255, 255, alpha));
				}
				pixels += bitmap.pitch;
			}
		} else {
			// 8 bits grey levels
			for(U32 y=0; y<height; ++y) {
				for(U32 x=0; x<width; ++x) {

					// the whole texture is white, just the alpha channel is changing
					U8 alpha = pixels[x];
					texImage.SetPixel(xOffset + x, yOffset + y, Color(255, 255, 255, alpha));
				}
				pixels += bitmap.pitch;
			}
		}

		// Release the old texture and replace it with the new
		ri::ServiceLocator::TexMgr().ReleaseTexture(page.texture);
		page.texture = ri::ServiceLocator::TexMgr().LoadTextureFromImage(texImage);
	}

	FT_Done_Glyph(glyphDesc);

	// return the final glyph
	return glyph;
}


IntRect Font::FindFittingTextureRect( Page &page, U32 width, U32 height ) const 
{
	// find fitting row
	Row *row = NULL;
	float bestRation = 0.0f;
	for(RowVector::iterator it = page.rows.begin(); it != page.rows.end() && !row; ++it) {
		float ratio = static_cast<float>(height) / static_cast<float>(it->height);

		if((ratio < 0.6f) || (ratio > 1.0f)) continue;

		// is there enough space left for this character?
		if(width > (page.textureImage.GetWidth() - it->width)) continue;

		// is it the best row so far?
		if(ratio < bestRation) continue;

		row = &(*it);
		bestRation = ratio;
	}

	// create a new row if no compatible was found
	if(!row) {
		Image &image = page.textureImage;

		U32 rowHeight = height + height / 10;

		// resize the texture if necessary
		while((rowHeight + page.nextRowY >= image.GetHeight()) || (width >= image.GetWidth())) {
			image.Resize(image.GetWidth() * 2, image.GetHeight() * 2, Color(255, 255, 255, 0));
		}

		page.rows.push_back(Row(page.nextRowY, rowHeight));
		page.nextRowY += rowHeight;
		row = &page.rows.back();
	}

	IntRect rect(row->width, row->top, width, height);

	// adjust the row
	row->width += width;

	return rect;
}


bool Font::SetCurrentSize( U32 fontSize ) const
{
	FT_Face face = static_cast<FT_Face>(mFace);
	FT_UShort currentSize = face->size->metrics.x_ppem;

	if (currentSize != fontSize) {
		FT_Error result = FT_Set_Pixel_Sizes(face, 0, fontSize);

		if (result == FT_Err_Invalid_Pixel_Size) {
			// In the case of bitmap fonts, resizing can
			// fail if the requested size is not available
			if (!FT_IS_SCALABLE(face))
			{
				std::string msg = "The font: " + mFontName + "doesn't support the text size: " + std::to_string(fontSize);
				MessageBox(NULL, "msg", "error", MB_OK);
				MessageBox(NULL, "implement a new error system", "TODO", MB_OK);
/*				err() << "Failed to set bitmap font size to " << characterSize << std::endl;
				err() << "Available sizes are: ";
				for (int i = 0; i < face->num_fixed_sizes; ++i)
					err() << face->available_sizes[i].height << " ";
				err() << std::endl; */
			}
		}

		if(result == FT_Err_Ok) return true;
		else					return false;
	} else {
		return true;
	}
}





Font::Page::Page() : glyphs(), textureImage(), texture(), rows(), nextRowY(3)
{
	textureImage.Create(128, 128, Color(255, 255, 255, 0));

	// Set a 2x2 rectangle for underlines
	textureImage.SetPixel(0, 0, ri::Color(255, 255, 255, 255));
	textureImage.SetPixel(1, 0, ri::Color(255, 255, 255, 255));
	textureImage.SetPixel(0, 1, ri::Color(255, 255, 255, 255));
	textureImage.SetPixel(1, 1, ri::Color(255, 255, 255, 255));

	texture = ri::ServiceLocator::TexMgr().LoadTextureFromImage(textureImage);
}


void Font::SetFontFilesDirectory( const std::string &directory )
{
	mFontFilesDirectory = directory;
}

} // namespace ri