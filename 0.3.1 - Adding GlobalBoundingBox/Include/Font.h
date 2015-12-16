#pragma once

/********************************************************************/
// INCLUDES
/********************************************************************/
#include "HandleMgr.h"
#include "Rectangle.h"
#include "Image.h"

#include <map>
#include <vector>


/********************************************************************/
// TODO: Implement correct error messaging
/********************************************************************/


namespace ri {


////////////////////////////////////////////////////////////
/// \brief Represents a character. Used to draw text
///
////////////////////////////////////////////////////////////
class RI_EXPORT Glyph {
public:
	Glyph() : AdvanceX(0), TexCoords(), BoundingBox() {}
	
	IntRect		TexCoords;		// texture coordinates in the font texture
	FloatRect	BoundingBox;	// Rectangle of the glyph relative to the base line 
	float		AdvanceX;		// Advance by this offset after rendering this glyph
};


////////////////////////////////////////////////////////////
/// \brief A font that can be loaded from file. Non-Bitmap fonts support font scaling
///
////////////////////////////////////////////////////////////
class RI_EXPORT Font {
public:

	Font();
	~Font();

	////////////////////////////////////////////////////////////
	/// \brief Load a font from file, supported files: ttf
	///
	////////////////////////////////////////////////////////////
	bool LoadFromFile(const std::string &filename);

	////////////////////////////////////////////////////////////
	/// \brief Destroys this font instance
	///
	////////////////////////////////////////////////////////////
	void Destroy();

	////////////////////////////////////////////////////////////
	/// \brief Returns the desired glyph, if the glyph can't be loaded it will return an empty glyph
	///
	////////////////////////////////////////////////////////////
	const Glyph& GetGlyph(U32 unicode, U32 fontSize, bool bold) const;

	////////////////////////////////////////////////////////////
	/// \brief Returns the font texture for this font/fontsize
	///
	////////////////////////////////////////////////////////////
	HTexture GetTexture(U32 fontSize) const;

	////////////////////////////////////////////////////////////
	/// \brief Returns the width of the image/texture of the bitmap with the fontsize
	///
	////////////////////////////////////////////////////////////
	U32	GetTextureSize(U32 fontSize) const;	

	////////////////////////////////////////////////////////////
	/// \brief Returns the Y-Offset to apply when printing a new line of text
	///
	////////////////////////////////////////////////////////////
	float GetYOffset(U32 fontSize) const; 

	////////////////////////////////////////////////////////////
	/// \brief Returns the y-distance from the baseline to the middle of the underline
	///
	////////////////////////////////////////////////////////////
	float GetUnderlineOffset(U32 fontSize) const;	

	////////////////////////////////////////////////////////////
	/// \brief Returns the thickness of the underline for the specified fontSize
	///
	////////////////////////////////////////////////////////////
	float GetUnderlineThickness(U32 fontSize) const;


	////////////////////////////////////////////////////////////
	/// \brief This will only affect fonts created after calling this function
	///
	////////////////////////////////////////////////////////////
	static void SetFontFilesDirectory(const std::string &directory);

private:

	////////////////////////////////////////////////////////////
	/// \brief Represents one row in a font image
	///
	////////////////////////////////////////////////////////////
	class Row {
	public:
		Row(U32 _top, U32 _height) : width(0), top(_top), height(_height) {}

		U32 width;			// actual width of this row
		U32 top;			// top y position
		U32 height;			// height of this row
	};

	// for clarity
	typedef std::map<U32, Glyph> GlyphMap;
	typedef std::vector<Row>	 RowVector;

	////////////////////////////////////////////////////////////
	/// \brief Defines the Data for one size of the font
	///
	////////////////////////////////////////////////////////////
	class Page {
	public:
		Page();

		GlyphMap	glyphs;				// maps char codes to glyphs
		Image		textureImage;		// texture of the glyphs in system memory
		HTexture	texture;			// texture of the glyphs in vram
		RowVector	rows;				// vector containing all rows
		U32			nextRowY;			// y position of the next free row
	};


	// map for the font sizes
	typedef std::map<U32, Page> PageMap;



	void	   *mLibrary;
	void	   *mFace;

	mutable PageMap	mFontData;
	std::string		mFontName;

	static std::string mFontFilesDirectory;

private:
	////////////////////////////////////////////////////////////
	/// \brief Loads the glyph into ram/vram and returns a glyph describing it
	///
	////////////////////////////////////////////////////////////
	Glyph	LoadGlyph(U32 unicode, U32 fontSize, bool bold) const;

	////////////////////////////////////////////////////////////
	/// \brief Sets the current font size if necessary
	///
	////////////////////////////////////////////////////////////
	bool	SetCurrentSize(U32 fontSize) const;

	////////////////////////////////////////////////////////////
	/// \brief Finds a free rect in the font-texture of page. It will resize the texture if necessary 
	///
	////////////////////////////////////////////////////////////
	IntRect FindFittingTextureRect(Page &page, U32 width, U32 height) const;
};


} // namespace ri