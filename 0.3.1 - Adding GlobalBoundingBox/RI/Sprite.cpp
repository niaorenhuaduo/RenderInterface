#include "..\Include\Drawable\Sprite.h"
#include "..\Include\ServiceLocator.h"
#include "..\Include\RenderState.h"

namespace ri
{


Sprite::Sprite() : Drawable(), Transformable() {
	Clear();
}

Sprite::Sprite( HTexture tex, bool bAlpha ) : Drawable(), Transformable() {
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

		ServiceLocator::RenderSystem().DrawPic(&mVertices[0], RenderState(transform, mTexture, mbAlpha));
	}
}

void Sprite::SetTexture( HTexture tex ) {
	mTexture = tex;
	mTexWidth  = ServiceLocator::TexMgr().GetWidth(tex);
	mTexHeight = ServiceLocator::TexMgr().GetHeight(tex);
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
	mVertices[1].mPos = Vec2(box.left + box.width,	box.top);
	mVertices[2].mPos = Vec2(box.left + box.width,	box.top + box.height);
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

ri::FloatRect Sprite::GetGlobalBoundingBox() const
{
	return GetTransformation().TransformRect(GetLocalBoundingBox());
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