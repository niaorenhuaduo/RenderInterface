#include "..\Include\Sprite.h"
#include "..\Include\ServiceLocator.h"
#include "..\Include\RenderState.h"

namespace ri
{


Sprite::Sprite() : Transformable() {
	Clear();
}

Sprite::Sprite( HTexture tex, bool bAlpha ) : Transformable() {
	Clear();
	SetTexture(tex);
	SetAlpha(bAlpha);
}

Sprite::Sprite( HTexture tex, const IntRect &textureRect, bool bAlpha ) : Transformable() {
	Clear();
	SetTexture(tex);
	SetTextureRect(textureRect);
	SetAlpha(bAlpha);
}

Sprite::Sprite(HTexture tex, U32 frameWidth, U32 frameHeight, U32 numColumns, U32 startFrame, U32 endFrame, double frameDelay, bool bLooped, bool bAlpha) : Transformable() {
	Clear();
	SetTexture(tex);
	SetAlpha(bAlpha);

	SetAnimation(frameWidth, frameHeight, numColumns, startFrame, endFrame, frameDelay, bLooped);	
}

Sprite::~Sprite() {}


void Sprite::Clear() {
	mTexture		 = HTexture();
	mbAlpha			 = false;
	mbFlipHorizontal = false;
	mbFlipVertical   = false;
	mbVisible		 = true;
	mbAnimated		 = false;
	mNumCols		 = 1;

	mVertices[0].mColor = Color::White;
	mVertices[1].mColor = Color::White;
	mVertices[2].mColor = Color::White;
	mVertices[3].mColor = Color::White;
}

void Sprite::OnUpdate(double fElapsedTime) {
	if(mbAnimated) mAnimation.OnUpdate(fElapsedTime);
}

void Sprite::OnDraw() {
	if(GetVisible() && !mTexture.IsNull()) {
		if(GetAnimated()) {
			// Update the TextureRect based on the current frame
			U32 curFrame = mAnimation.GetCurrentFrame();
			UpdateTextureRectPositionOnAnimation(curFrame);
		}

		Matrix2D transform = GetTransformation();		// Transformation of the sprite
		Matrix2D flipmat;								// Apply the flipping in front of the actual transformation

		if(GetFlippingHorizontal()) {
			flipmat.Scale(-1.0f, 1.0f);
			flipmat.Translate(static_cast<float>(mTextureRect.width), 0.0f);
		}

		if(GetFlippingVertical()) {
			flipmat.Scale(1.0f, -1.0f);
			flipmat.Translate(0.0f, static_cast<float>(mTextureRect.height));
		}
		transform = flipmat * transform;

		ServiceLocator::RenderSystem().VDrawPic(&mVertices[0], RenderState(transform, mTexture, mbAlpha));

/*
		Vec2 topleftpos  = mVertices[0].pos * transform;
		Vec2 toprightpos = mVertices[1].pos * transform;
		Vec2 botrightpos = mVertices[2].pos * transform;
		Vec2 botleftpos  = mVertices[3].pos * transform;

		Vec4 topleft     = Vec4(topleftpos.x, topleftpos.y, mVertices[0].uv.x, mVertices[0].uv.y);
		Vec4 topright    = Vec4(toprightpos.x, toprightpos.y, mVertices[1].uv.x, mVertices[1].uv.y);
		Vec4 bottomright = Vec4(botrightpos.x, botrightpos.y, mVertices[2].uv.x, mVertices[2].uv.y);
		Vec4 bottomleft  = Vec4(botleftpos.x, botleftpos.y, mVertices[3].uv.x, mVertices[3].uv.y);

		// The quad has to point to the camera
		if(GetFlippingHorizontal()) {
			if(GetFlippingVertical())	pRI->VDrawPic(topleft, topright, bottomright, bottomleft, mTexture, mbAlpha);
			else						pRI->VDrawPic(topright, topleft, bottomleft, bottomright, mTexture, mbAlpha);
		} else {
			if(GetFlippingVertical())	pRI->VDrawPic(topright, topleft, bottomleft, bottomright, mTexture, mbAlpha);
			else						pRI->VDrawPic(topleft, topright, bottomright, bottomleft, mTexture, mbAlpha);
		}*/
	}
}

void Sprite::SetTexture( HTexture tex ) {
	mTexture = tex;
	mTexWidth  = ServiceLocator::TexMgr().VGetWidth(tex);
	mTexHeight = ServiceLocator::TexMgr().VGetHeight(tex);
	mNumCols = 1;

	IntRect texRect(0, 0, mTexWidth, mTexHeight);

	SetTextureRect(texRect);
}

void Sprite::SetTextureRect( const IntRect &texRect ){
	if(mTexture.IsNull() || texRect.width == 0 || texRect.height == 0) return;

	// Setting the texture rect disables animations
	mbAnimated = false;

	mTextureRect.left   = texRect.left;
	mTextureRect.top    = texRect.top;
	mTextureRect.width  = std::abs(texRect.width);
	mTextureRect.height = std::abs(texRect.height);

	UpdatePositions();
	UpdateTextureCoordinates();
}


void Sprite::SetAnimation( U32 frameWidth, U32 frameHeight, U32 numColumns, U32 startFrame, U32 endFrame, double frameDelay, bool bLooped /* = false */ ) {
	if(frameWidth == 0 || frameHeight == 0) return;
	if(startFrame >= endFrame || numColumns == 0 || frameDelay <= 0) return;

	mbAnimated = true;
	mAnimation.Init(startFrame, endFrame, frameDelay, bLooped);
	mNumCols = numColumns;

	// update the size of the texture rect to the frame size
	mTextureRect.width  = frameWidth;
	mTextureRect.height = frameHeight;
	UpdatePositions();
}


void Sprite::UpdatePositions() {
	FloatRect box = GetLocalBoundingBox();

	mVertices[0].mPos = Vec2(box.left,				box.top);			
	mVertices[1].mPos = Vec2(box.left + box.width, box.top);
	mVertices[2].mPos = Vec2(box.left + box.width, box.top + box.height);
	mVertices[3].mPos = Vec2(box.left,				box.top + box.height);
}

void Sprite::UpdateTextureCoordinates() {
	float left	 = static_cast<float>(mTextureRect.left)						/ static_cast<float>(mTexWidth);
	float right  = static_cast<float>(mTextureRect.left + mTextureRect.width) 	/ static_cast<float>(mTexWidth);
	float top	 = static_cast<float>(mTextureRect.top) 						/ static_cast<float>(mTexHeight);
	float bottom = static_cast<float>(mTextureRect.top + mTextureRect.height)	/ static_cast<float>(mTexHeight);

	mVertices[0].mTex = Vec2(left,  top);
	mVertices[1].mTex = Vec2(right, top);
	mVertices[2].mTex = Vec2(right, bottom);
	mVertices[3].mTex = Vec2(left,  bottom);
}


void Sprite::UpdateTextureRectPositionOnAnimation( U32 frame )
{
	mTextureRect.left = (frame % mNumCols) * mTextureRect.width;
	mTextureRect.top  = (frame / mNumCols) * mTextureRect.height;

	UpdateTextureCoordinates();
}


FloatRect Sprite::GetLocalBoundingBox() const {
	return FloatRect(0.0f, 0.0f, static_cast<float>(mTextureRect.width), static_cast<float>(mTextureRect.height));
}

/*RIFloatRect Sprite::GetGlobalBoundingBox() const {

}*/


//============================ ANIMATION ========================================//
Sprite::Animation::Animation() : mStartFrame(0), mEndFrame(0), mCurrentFrame(0), mFrameDelay(1.0), mAnimTimer(0.0), mbLooped(false)
{
}

void Sprite::Animation::Init( U32 start, U32 end, double frameDelay, bool bLooped /* =false */ ) 
{
	mStartFrame = start;
	mEndFrame = end;
	mCurrentFrame = start;
	mFrameDelay = frameDelay;
	mAnimTimer = 0.0;
	mbLooped = bLooped;
}

Sprite::Animation::~Animation() {}

void Sprite::Animation::OnUpdate(double fElapsedTime)
{
	if(mStartFrame == mEndFrame) return;

	mAnimTimer += fElapsedTime;
	if(mAnimTimer > mFrameDelay) {
		mAnimTimer -= mFrameDelay;
		++mCurrentFrame;
		if(mCurrentFrame < mStartFrame || mCurrentFrame > mEndFrame) {
			if(mbLooped) mCurrentFrame = mStartFrame;
			else         mCurrentFrame = mEndFrame;
		}
	}
}




} // namespace ri