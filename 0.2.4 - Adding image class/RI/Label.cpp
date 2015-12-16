#include "..\Include\Label.h"
#include "..\Include\ServiceLocator.h"
#include "..\Include\IRenderSystem.h"
#include "..\Include\RenderState.h"


namespace ri {


Label::Label() : mText(), mpFont(NULL), mTextSize(24), mColor(Color::White), mBoundingBox(), mVertices(), mbGeometryNeedsUpdate(true),
	mbItalic(false), mbUnderlined(false), mbBold(false)
{}


Label::Label(const std::string &text, const Font &font, U32 textSize) : mText(text), mpFont(&font), mTextSize(textSize), mColor(Color::White), mBoundingBox(), 
	mVertices(), mbGeometryNeedsUpdate(true), mbItalic(false), mbUnderlined(false), mbBold(false)
{}


void Label::OnDraw() {
	if(mpFont && mVertices.size() != 0) {

		RenderState state(GetTransformation(), mpFont->GetTexture(mTextSize), true);
		ServiceLocator::RenderSystem()._Draw(&mVertices[0], mVertices.size(), PRIMITIVETYPE_TRIANGLE, state);
	}
}


void Label::RecalcGeometry() {
	if(mText.empty())	return;
	if(!mpFont)			return;
	if(mTextSize == 0)	return;

	// clear the old
	mVertices.clear();
	mBoundingBox = FloatRect();
	mbGeometryNeedsUpdate = false;


	float x = 0.0f;
	float y = static_cast<float>(mTextSize);

	float xOffset = mpFont->GetGlyph(' ', mTextSize, mbBold).AdvanceX;
	float yOffset = mpFont->GetYOffset(mTextSize);

	// bounding box
	float minX = static_cast<float>(mTextSize);
	float minY = static_cast<float>(mTextSize);
	float maxX = 0.0f;
	float maxY = 0.0f;

	for(U32 i=0; i<mText.length(); ++i) {
		U32 c = mText.at(i);

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

		HTexture tex   = mpFont->GetTexture(mTextSize);
		float texWidth   = static_cast<float>(ri::ServiceLocator::TexMgr().GetWidth(tex));
		float texHeight  = static_cast<float>(ri::ServiceLocator::TexMgr().GetHeight(tex));

		float left		= g.BoundingBox.left;
		float top		= g.BoundingBox.top;
		float right		= left + g.BoundingBox.width;
		float bottom	= top  + g.BoundingBox.height;

		float u1 = static_cast<float>(g.TexCoords.left) / texWidth;
		float v1 = static_cast<float>(g.TexCoords.top)  / texHeight;
		float u2 = static_cast<float>(g.TexCoords.left + g.TexCoords.width)  / texWidth;
		float v2 = static_cast<float>(g.TexCoords.top  + g.TexCoords.height) / texHeight;

		mVertices.push_back(Vertex2D(Vec2(x + left,  y + top),		Vec2(u1, v1), mColor));
		mVertices.push_back(Vertex2D(Vec2(x + right, y + top),		Vec2(u2, v1), mColor));
		mVertices.push_back(Vertex2D(Vec2(x + left,  y + bottom),	Vec2(u1, v2), mColor));

		mVertices.push_back(Vertex2D(Vec2(x + left,  y + bottom),	Vec2(u1, v2), mColor));
		mVertices.push_back(Vertex2D(Vec2(x + right, y + top),		Vec2(u2, v1), mColor));
		mVertices.push_back(Vertex2D(Vec2(x + right, y + bottom),	Vec2(u2, v2), mColor));

		// Update bounding box
		minX = std::min(minX, x + left);
		minY = std::min(minY, y + top);
		maxX = std::max(maxX, x + right);
		maxY = std::max(maxY, y + bottom);

		x += g.AdvanceX;
	}


	mBoundingBox.left	= minX;
	mBoundingBox.top	= minY;
	mBoundingBox.width  = maxX - minX;
	mBoundingBox.height = maxY - minY;
}



} // namespace ri