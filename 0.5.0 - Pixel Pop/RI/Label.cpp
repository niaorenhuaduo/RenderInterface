#include "..\Include\Drawable\Label.h"
#include "..\Include\ServiceLocator.h"
#include "..\Include\IRenderSystem.h"
#include "..\Include\RenderState.h"


namespace ri {


Label::Label() : Drawable(), Transformable(), mText(), mpFont(NULL), mTextSize(24), mColor(Color::White), mBoundingBox(), mVertices(), mbGeometryNeedsUpdate(true),
	mbUnderlined(false), mbBold(false), mActTextureSize(0), mbShadowed(false), mShadowOffset(0.0f), mShadowColor(Color::Black)
{}


Label::Label(const std::string &text, const Font &font, U32 textSize) : Drawable(), Transformable(), mText(text), mpFont(&font), mTextSize(textSize), mColor(Color::White), mBoundingBox(), 
	mVertices(), mbGeometryNeedsUpdate(true), mbUnderlined(false), mbBold(false), mActTextureSize(0), mbShadowed(false), mShadowOffset(0.0f), mShadowColor(Color::Black)
{}


void Label::OnDraw() {
	if(mbGeometryNeedsUpdate) RecalcGeometry();

	if(mpFont && mVertices.size() != 0) {

		// update the texture coordinates if the texture size doesn't match, this happens if another label uses the same font and updates the font texture
		U32 fontTextureSize = mpFont->GetTextureSize(mTextSize);
		if(fontTextureSize != mActTextureSize) {
			RecalcGeometry();
		}

		RenderState state(GetTransformation(), mpFont->GetTexture(mTextSize), true);
		ServiceLocator::RenderSystem()._Draw(&mVertices[0], mVertices.size(), PRIMITIVETYPE_TRIANGLE, state);
	}
}


void Label::RecalcGeometry() const {
	if(!mpFont)			return;
	if(mTextSize == 0)	return;

	// clear the old
	mVertices.clear();
	mBoundingBox = FloatRect();

	float x = 0.0f;
	float y = -mpFont->GetGlyph('T', mTextSize, mbBold).BoundingBox.top;

	float xOffset = mpFont->GetGlyph(' ', mTextSize, mbBold).AdvanceX;
	float yOffset = mpFont->GetYOffset(mTextSize);

	Vec2 shadowOffset = (mShadowOffset == Vec2(0.0f)) ? Vec2(std::floor(static_cast<float>(mTextSize) / 20.0f + 0.5f)) : mShadowOffset;

	float underlineOffset		= mpFont->GetUnderlineOffset(mTextSize);						
	float underlineThickness	= mpFont->GetUnderlineThickness(mTextSize) + underlineOffset;

	// bounding box
	float minX = static_cast<float>(mTextSize);
	float minY = static_cast<float>(mTextSize);
	float maxX = 0.0f;
	float maxY = 0.0f;

	for(U32 i=0; i<mText.length(); ++i) {
		U32 c = mText.at(i);

		// if we have a new line and we draw the text underlined, draw the line
		if(mbUnderlined && (c == '\n')) {
			float top = std::floor(y + underlineOffset - (underlineThickness / 2.0f) + 0.5f);
			float bot = top + std::floor(underlineThickness + 0.5f);

			if(mbShadowed) {
				mVertices.push_back(Vertex2D(Vec2(  - 0.5f,	bot - 0.5f) + shadowOffset, Vec2(1.0f, 1.0f), mShadowColor));
				mVertices.push_back(Vertex2D(Vec2(  - 0.5f,	top - 0.5f) + shadowOffset, Vec2(1.0f, 1.0f), mShadowColor));
				mVertices.push_back(Vertex2D(Vec2(x - 0.5f, top - 0.5f) + shadowOffset, Vec2(1.0f, 1.0f), mShadowColor));

				mVertices.push_back(Vertex2D(Vec2(  - 0.5f,	bot - 0.5f) + shadowOffset, Vec2(1.0f, 1.0f), mShadowColor));
				mVertices.push_back(Vertex2D(Vec2(x - 0.5f, top - 0.5f) + shadowOffset, Vec2(1.0f, 1.0f), mShadowColor));
				mVertices.push_back(Vertex2D(Vec2(x - 0.5f, bot - 0.5f) + shadowOffset, Vec2(1.0f, 1.0f), mShadowColor));

				maxX = std::max(maxX, x + shadowOffset.x);
			}

			mVertices.push_back(Vertex2D(Vec2(  - 0.5f,	bot - 0.5f), Vec2(1.0f, 1.0f), mColor));
			mVertices.push_back(Vertex2D(Vec2(  - 0.5f,	top - 0.5f), Vec2(1.0f, 1.0f), mColor));
			mVertices.push_back(Vertex2D(Vec2(x - 0.5f, top - 0.5f), Vec2(1.0f, 1.0f), mColor));

			mVertices.push_back(Vertex2D(Vec2(  - 0.5f,	bot - 0.5f), Vec2(1.0f, 1.0f), mColor));
			mVertices.push_back(Vertex2D(Vec2(x - 0.5f, top - 0.5f), Vec2(1.0f, 1.0f), mColor));
			mVertices.push_back(Vertex2D(Vec2(x - 0.5f, bot - 0.5f), Vec2(1.0f, 1.0f), mColor));

			maxX = std::max(maxX, x);
		}

		// This characters don't require quads
		if((c == ' ') || (c =='\n') || (c == '\t')) {
			minX = std::min(minX, x);
			minY = std::min(minY, y);

			switch(c) 
			{
				case ' ':  x += xOffset;			break;
				case '\n': y += yOffset; x = 0.0f;	break;
				case '\t': x += 4 * xOffset;		break;
			}

			maxX = std::max(maxX, x);
			maxY = std::max(maxY, y);

			continue;
		}

		// calc the glyph geometry
		const Glyph &g = mpFont->GetGlyph(c, mTextSize, mbBold);

		float left		= g.BoundingBox.left;
		if(i == 0) left = 0.0f;
		float top		= g.BoundingBox.top;
		float right		= left + g.BoundingBox.width;
		float bottom	= top  + g.BoundingBox.height; 

		float u1 = static_cast<float>(g.TexCoords.left);
		float v1 = static_cast<float>(g.TexCoords.top);
		float u2 = static_cast<float>(g.TexCoords.left + g.TexCoords.width);
		float v2 = static_cast<float>(g.TexCoords.top  + g.TexCoords.height);

		// subtracting 0.5 for optimal texel to screen mapping
		Vec2 topleft	= Vec2(x + left  - 0.5f, y + top	- 0.5f);
		Vec2 topright	= Vec2(x + right - 0.5f, y + top	- 0.5f);
		Vec2 botleft	= Vec2(x + left  - 0.5f, y + bottom - 0.5f);
		Vec2 botright	= Vec2(x + right - 0.5f, y + bottom - 0.5f);

		if(mbShadowed) {
			mVertices.push_back(Vertex2D(topleft  + shadowOffset,	Vec2(u1, v1), mShadowColor));
			mVertices.push_back(Vertex2D(topright + shadowOffset,	Vec2(u2, v1), mShadowColor));
			mVertices.push_back(Vertex2D(botleft  + shadowOffset,	Vec2(u1, v2), mShadowColor));

			mVertices.push_back(Vertex2D(botleft  + shadowOffset,	Vec2(u1, v2), mShadowColor));
			mVertices.push_back(Vertex2D(topright + shadowOffset,	Vec2(u2, v1), mShadowColor));
			mVertices.push_back(Vertex2D(botright + shadowOffset,	Vec2(u2, v2), mShadowColor));

			minX = std::min(minX, x + left   + shadowOffset.x);
			minY = std::min(minY, y + top    + shadowOffset.y);
			maxX = std::max(maxX, x + right  + shadowOffset.x);
			maxY = std::max(maxY, y + bottom + shadowOffset.y);
		}

		mVertices.push_back(Vertex2D(topleft,	Vec2(u1, v1), mColor));
		mVertices.push_back(Vertex2D(topright,	Vec2(u2, v1), mColor));
		mVertices.push_back(Vertex2D(botleft,	Vec2(u1, v2), mColor));

		mVertices.push_back(Vertex2D(botleft,	Vec2(u1, v2), mColor));
		mVertices.push_back(Vertex2D(topright,	Vec2(u2, v1), mColor));
		mVertices.push_back(Vertex2D(botright,	Vec2(u2, v2), mColor));

		// Update bounding box
		minX = std::min(minX, x + left);
		minY = std::min(minY, y + top);
		maxX = std::max(maxX, x + right);
		maxY = std::max(maxY, y + bottom);

		x += g.AdvanceX;
		if(i == 0) x -= g.BoundingBox.left;
	}

	// draw the underline for the last line
	if(mbUnderlined) {
		float top = std::floor(y + underlineOffset - (underlineThickness / 2.0f) + 0.5f);
		float bot = top + std::floor(underlineThickness + 0.5f);

		if(mbShadowed) {
			mVertices.push_back(Vertex2D(Vec2(  - 0.5f,	bot - 0.5f) + shadowOffset, Vec2(1.0f, 1.0f), mShadowColor));
			mVertices.push_back(Vertex2D(Vec2(  - 0.5f,	top - 0.5f) + shadowOffset, Vec2(1.0f, 1.0f), mShadowColor));
			mVertices.push_back(Vertex2D(Vec2(x - 0.5f, top - 0.5f) + shadowOffset, Vec2(1.0f, 1.0f), mShadowColor));

			mVertices.push_back(Vertex2D(Vec2(  - 0.5f,	bot - 0.5f) + shadowOffset, Vec2(1.0f, 1.0f), mShadowColor));
			mVertices.push_back(Vertex2D(Vec2(x - 0.5f, top - 0.5f) + shadowOffset, Vec2(1.0f, 1.0f), mShadowColor));
			mVertices.push_back(Vertex2D(Vec2(x - 0.5f, bot - 0.5f) + shadowOffset, Vec2(1.0f, 1.0f), mShadowColor));

			maxX = std::max(maxX, x   + shadowOffset.x);
			maxY = std::max(maxY, bot + shadowOffset.y);
		}

		mVertices.push_back(Vertex2D(Vec2(  - 0.5f,	bot - 0.5f), Vec2(1.0f, 1.0f), mColor));
		mVertices.push_back(Vertex2D(Vec2(  - 0.5f,	top - 0.5f), Vec2(1.0f, 1.0f), mColor));
		mVertices.push_back(Vertex2D(Vec2(x - 0.5f, top - 0.5f), Vec2(1.0f, 1.0f), mColor));

		mVertices.push_back(Vertex2D(Vec2(  - 0.5f,	bot - 0.5f), Vec2(1.0f, 1.0f), mColor));
		mVertices.push_back(Vertex2D(Vec2(x - 0.5f, top - 0.5f), Vec2(1.0f, 1.0f), mColor));
		mVertices.push_back(Vertex2D(Vec2(x - 0.5f, bot - 0.5f), Vec2(1.0f, 1.0f), mColor));

		maxX = std::max(maxX, x);
		maxY = std::max(maxY, bot);
	}


	mBoundingBox.left	= minX;
	mBoundingBox.top	= minY;
	mBoundingBox.width  = maxX - minX;
	mBoundingBox.height = maxY - minY;


	// finish the texture coordinates
	HTexture tex   = mpFont->GetTexture(mTextSize);
	float texWidth   = static_cast<float>(ri::ServiceLocator::TexMgr().GetWidth(tex));
	float texHeight  = static_cast<float>(ri::ServiceLocator::TexMgr().GetHeight(tex));

	for(auto it = mVertices.begin(); it != mVertices.end(); ++it) {
		it->mTex.x /= texWidth;
		it->mTex.y /= texHeight;
	}

	// save actual texture size
	mActTextureSize = static_cast<U32>(texWidth);

	mbGeometryNeedsUpdate = false;
}


void Label::SetColor( const Color &color )
{
	mColor = color;

	if(!mbGeometryNeedsUpdate) {
		if(!mbShadowed) for(auto it = mVertices.begin(); it != mVertices.end(); ++it) it->mColor = color;
		else			mbGeometryNeedsUpdate = true;
	}
}


void Label::SetText( const std::string &text )
{
	if(mText != text) {
		mText = text;
		mbGeometryNeedsUpdate = true;
	}
}

void Label::SetBold( bool bold )
{
	if(mbBold != bold) {
		mbBold = bold;
		mbGeometryNeedsUpdate = true;
	}
}

void Label::SetUnderlined( bool bUnderlined )
{
	if(mbUnderlined != bUnderlined) {
		mbUnderlined = bUnderlined;
		mbGeometryNeedsUpdate = true;
	}
}

void Label::SetFont( const Font &newFont )
{
	if(mpFont != &newFont) {
		mpFont = &newFont;
		mbGeometryNeedsUpdate = true;
	}
}

void Label::SetTextSize( U32 textSize )
{
	if((mTextSize != textSize) && textSize > 0) {
		mTextSize = textSize;
		mbGeometryNeedsUpdate = true;
	}
}

void Label::SetShadowed( bool bShadowed, const Color &shadowColor /*= Color::Black*/, const Vec2 &shadowOffset /*= 0.0f*/ )
{
	mbShadowed		= bShadowed;
	mShadowColor	= shadowColor;
	mShadowOffset	= shadowOffset;

	mbGeometryNeedsUpdate = true;
}

FloatRect Label::GetLocalBoundingBox() const
{
	if(mbGeometryNeedsUpdate) RecalcGeometry();

	if(mVertices.size() > 0) return mBoundingBox;
	else					 return FloatRect();
}


ri::FloatRect Label::GetGlobalBoundingBox() const
{
	return GetTransformation().TransformRect(GetLocalBoundingBox());
}



} // namespace ri