#include "..\Include\Drawable\RectangleShape.h"

#include "..\Include\RenderState.h"
#include "..\Include\IRenderSystem.h"
#include "..\Include\ServiceLocator.h"

namespace ri {

RectangleShape::RectangleShape() : mSize(100.0f, 100.0f), mColor(Color::White), mBoundingBox() {
	RecalcGeometry();
}



RectangleShape::RectangleShape( float width, float height, const Color &color /*= Color::White*/ ) :
	mSize(width, height), mColor(color), mBoundingBox()
{
	if(mSize.x <= 0) mSize.x = 100.0f;
	if(mSize.y <= 0) mSize.y = 100.0f;

	RecalcGeometry();
}

void RectangleShape::OnDraw()
{
	RenderState state(GetTransformation(), HTexture(), true);

	ServiceLocator::RenderSystem()._Draw(mVertices, 6, PRIMITIVETYPE_TRIANGLE, state);
}

void RectangleShape::RecalcGeometry()
{
	mVertices[0].mPos = Vec2(-0.5f, mSize.y - 0.5f);
	mVertices[0].mTex = Vec2();
	mVertices[0].mColor = mColor;

	mVertices[1].mPos = Vec2(-0.5f, -0.5f);
	mVertices[1].mTex = Vec2();
	mVertices[1].mColor = mColor;

	mVertices[2].mPos = Vec2(mSize.x - 0.5f, - 0.5f);
	mVertices[2].mTex = Vec2();
	mVertices[2].mColor = mColor;

	mVertices[3].mPos = Vec2(-0.5f, mSize.y - 0.5f);
	mVertices[3].mTex = Vec2();
	mVertices[3].mColor = mColor;

	mVertices[4].mPos = Vec2(mSize.x - 0.5f, -0.5f);
	mVertices[4].mTex = Vec2();
	mVertices[4].mColor = mColor;

	mVertices[5].mPos = Vec2(mSize.x - 0.5f, mSize.y - 0.5f);
	mVertices[5].mTex = Vec2();
	mVertices[5].mColor = mColor;

	mBoundingBox.left	= 0.0f;
	mBoundingBox.top	= 0.0f;
	mBoundingBox.width  = mSize.x;
	mBoundingBox.height = mSize.y;
}


void RectangleShape::SetSize( const Vec2 &size )
{
	mSize = size;

	if(mSize.x <= 0) mSize.x = 100.0f;
	if(mSize.y <= 0) mSize.y = 100.0f;

	RecalcGeometry();
}

void RectangleShape::SetSize( float width, float height )
{
	SetSize(Vec2(width, height));
}

void RectangleShape::SetColor( const Color &color )
{
	for(U32 i=0; i<6; ++i) mVertices[i].mColor = color;
}

ri::FloatRect RectangleShape::GetLocalBoundingBox() const
{
	return mBoundingBox;
}

ri::FloatRect RectangleShape::GetGlobalBoundingBox() const
{
	return GetTransformation().TransformRect(GetLocalBoundingBox());
}


} // namespace ri