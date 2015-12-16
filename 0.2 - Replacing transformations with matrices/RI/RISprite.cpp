#include "..\Include\RISprite.h"
#include "..\Include\RIServiceLocator.h"

//************************************
// Method:    RISprite
// FullName:  RISprite::RISprite
// Access:    public 
// Returns:   vjnsdkgbdskgb
// Qualifier: : RITransformable()
//************************************
RISprite::RISprite() : RITransformable() {
	Clear();
}

RISprite::RISprite( HTexture tex, bool bAlpha ) : RITransformable() {
	Clear();
	SetTexture(tex);
	SetAlpha(bAlpha);
}

RISprite::RISprite( HTexture tex, const RIIntRect &textureRect, bool bAlpha ) : RITransformable() {
	Clear();
	SetTexture(tex);
	SetTextureRect(textureRect);
	SetAlpha(bAlpha);
}

RISprite::RISprite(HTexture tex, U32 frameWidth, U32 frameHeight, U32 numColumns, U32 startFrame, U32 endFrame, double frameDelay, bool bLooped, bool bAlpha) : RITransformable() {
	Clear();
	SetTexture(tex);
	SetAlpha(bAlpha);

	SetAnimation(frameWidth, frameHeight, numColumns, startFrame, endFrame, frameDelay, bLooped);	
}

RISprite::~RISprite() {}


void RISprite::Clear() {
	mTexture		 = HTexture();
	mbAlpha			 = false;
	mbFlipHorizontal = false;
	mbFlipVertical   = false;
	mbVisible		 = true;
	mbAnimated		 = false;
	mNumCols		 = 1;
}

void RISprite::OnUpdate(double fElapsedTime) {
	if(mbAnimated) mAnimation.OnUpdate(fElapsedTime);
}

void RISprite::OnDraw( RIRenderInterface *pRI ) {
	assert(pRI && "RISprite::OnDraw: NULL pointer!");

	if(GetVisible() && !mTexture.IsNull()) {
		if(GetAnimated()) {
			// Update the TextureRect based on the current frame
			U32 curFrame = mAnimation.GetCurrentFrame();
			UpdateTextureRectPositionOnAnimation(curFrame);
		}

		RIMatrix2D transform = GetTransformation();		// Transformation of the sprite
		RIMatrix2D flipmat;								// Apply the flipping in front of the actual transformation

		if(GetFlippingHorizontal()) {
			flipmat.Scale(-1.0f, 1.0f);
			flipmat.Translate(static_cast<float>(mTextureRect.width), 0.0f);
		}

		if(GetFlippingVertical()) {
			flipmat.Scale(1.0f, -1.0f);
			flipmat.Translate(0.0f, static_cast<float>(mTextureRect.height));
		}
		transform = flipmat * transform;

		RIVec2 topleftpos  = mVertices[0].pos * transform;
		RIVec2 toprightpos = mVertices[1].pos * transform;
		RIVec2 botrightpos = mVertices[2].pos * transform;
		RIVec2 botleftpos  = mVertices[3].pos * transform;

		RIVec4 topleft     = RIVec4(topleftpos.x, topleftpos.y, mVertices[0].uv.x, mVertices[0].uv.y);
		RIVec4 topright    = RIVec4(toprightpos.x, toprightpos.y, mVertices[1].uv.x, mVertices[1].uv.y);
		RIVec4 bottomright = RIVec4(botrightpos.x, botrightpos.y, mVertices[2].uv.x, mVertices[2].uv.y);
		RIVec4 bottomleft  = RIVec4(botleftpos.x, botleftpos.y, mVertices[3].uv.x, mVertices[3].uv.y);

		// The quad has to point to the camera
		if(GetFlippingHorizontal()) {
			if(GetFlippingVertical())	pRI->VDrawPic(topleft, topright, bottomright, bottomleft, mTexture, mbAlpha);
			else						pRI->VDrawPic(topright, topleft, bottomleft, bottomright, mTexture, mbAlpha);
		} else {
			if(GetFlippingVertical())	pRI->VDrawPic(topright, topleft, bottomleft, bottomright, mTexture, mbAlpha);
			else						pRI->VDrawPic(topleft, topright, bottomright, bottomleft, mTexture, mbAlpha);
		}
	}
}

void RISprite::SetTexture( HTexture tex ) {
	mTexture = tex;
	mTexWidth  = RIServiceLocator::GetTexMgr().VGetWidth(tex);
	mTexHeight = RIServiceLocator::GetTexMgr().VGetHeight(tex);
	mNumCols = 1;

	RIIntRect texRect(0, 0, mTexWidth, mTexHeight);

	SetTextureRect(texRect);
}

void RISprite::SetTextureRect( const RIIntRect &texRect ){
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


void RISprite::SetAnimation( U32 frameWidth, U32 frameHeight, U32 numColumns, U32 startFrame, U32 endFrame, double frameDelay, bool bLooped /*= false*/ ) {
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


void RISprite::UpdatePositions() {
	RIFloatRect box = GetLocalBoundingBox();

	mVertices[0].pos = RIVec2(box.left,				box.top);			
	mVertices[1].pos = RIVec2(box.left + box.width, box.top);
	mVertices[2].pos = RIVec2(box.left + box.width, box.top + box.height);
	mVertices[3].pos = RIVec2(box.left,				box.top + box.height);
}

void RISprite::UpdateTextureCoordinates() {
	float left	 = static_cast<float>(mTextureRect.left)						/ static_cast<float>(mTexWidth);
	float right  = static_cast<float>(mTextureRect.left + mTextureRect.width) 	/ static_cast<float>(mTexWidth);
	float top	 = static_cast<float>(mTextureRect.top) 						/ static_cast<float>(mTexHeight);
	float bottom = static_cast<float>(mTextureRect.top + mTextureRect.height)	/ static_cast<float>(mTexHeight);

	mVertices[0].uv = RIVec2(left,  top);
	mVertices[1].uv = RIVec2(right, top);
	mVertices[2].uv = RIVec2(right, bottom);
	mVertices[3].uv = RIVec2(left,  bottom);
}


void RISprite::UpdateTextureRectPositionOnAnimation( U32 frame )
{
	mTextureRect.left = (frame % mNumCols) * mTextureRect.width;
	mTextureRect.top  = (frame / mNumCols) * mTextureRect.height;

	UpdateTextureCoordinates();
}


RIFloatRect RISprite::GetLocalBoundingBox() const {
	return RIFloatRect(0.0f, 0.0f, static_cast<float>(mTextureRect.width), static_cast<float>(mTextureRect.height));
}

/*RIFloatRect RISprite::GetGlobalBoundingBox() const {

}*/


//============================ ANIMATION ========================================//
RISprite::Animation::Animation() : mStartFrame(0), mEndFrame(0), mCurrentFrame(0), mFrameDelay(1.0), mAnimTimer(0.0), mbLooped(false)
{
}

void RISprite::Animation::Init( U32 start, U32 end, double frameDelay, bool bLooped/*=false*/ ) 
{
	mStartFrame = start;
	mEndFrame = end;
	mCurrentFrame = start;
	mFrameDelay = frameDelay;
	mAnimTimer = 0.0;
	mbLooped = bLooped;
}

RISprite::Animation::~Animation() {}

void RISprite::Animation::OnUpdate(double fElapsedTime)
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



