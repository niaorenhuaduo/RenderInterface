#include "..\Include\RISprite.h"

RISprite::RISprite() {
	mbInitialized = false;
	mpRI = NULL;
}

RISprite::~RISprite() {

}


void RISprite::ResetPositionAnimation()
{
	mPosition			= RIVec2(0.0f);
	mRelativeCenter		= RIVec2(0.0f);
	mbFlipVertical		= false;
	mbFlipHorizontal	= false;

	mbVisible			= true;
	mfRotation			= 0.0f;
	mScale				= RIVec2(1.0f);

	mStartFrame			= 0;
	mEndFrame			= 0;
	mNumColumns			= 1;
	mCurrentFrame		= 0;
	mTimePerFrame		= 1.0;
	mAnimationTimer		= 0.0;
	mbLoop				= false;
	mbAnimationComplete = true;
}

// No return value because this method can only succeed or shut down the program
void RISprite::VOnInit(RIRenderInterface *pRI, HTexture texture, const RECT *textureRect, bool bAlpha) {
	assert(pRI);
	assert(pRI->GetTexMgr());
	mpRI = pRI;

	ResetPositionAnimation();
	SetTexture(texture, textureRect, bAlpha);

	mbInitialized = true;
}

void RISprite::VOnInit( RIRenderInterface *pRI, HTexture texture, U32 twidth, U32 theight, U32 columns, U32 startFrame, U32 endFrame, double timePerFrame, bool bAlpha )
{
	assert(pRI);
	assert(pRI->GetTexMgr());
	mpRI = pRI;

	ResetPositionAnimation();

	// Initialize animation
	mStartFrame			= startFrame;
	mEndFrame			= endFrame;
	if(mEndFrame < mStartFrame) mEndFrame = mStartFrame;
	mNumColumns			= columns;
	if(mNumColumns == 0) mNumColumns = 1;
	mCurrentFrame		= startFrame;
	mTimePerFrame		= timePerFrame;
	if(mTimePerFrame < 0.0) mTimePerFrame = 1.0;
	mAnimationTimer		= 0.0;
	mbLoop				= true;
	mbAnimationComplete = false;

	RECT texRect;
	texRect.left = (mCurrentFrame % mNumColumns) * twidth;
	texRect.right = texRect.left + twidth;
	texRect.top = (mCurrentFrame / mNumColumns) * theight;
	texRect.bottom = texRect.top + theight;
	SetTexture(texture, &texRect, bAlpha);

	mbInitialized = true;
}

void RISprite::SetTexture(HTexture tex, const RECT *textureRect, bool bAlpha) {
	if(!mpRI) return;

	mTextureWidth = mpRI->GetTexMgr()->VGetWidth(tex);
	mTextureHeight = mpRI->GetTexMgr()->VGetHeight(tex);
	if(textureRect == NULL || (textureRect->right - textureRect->left) <=0 
						   || (textureRect->bottom - textureRect->top) <=0) {
		mWidth				= mTextureWidth;
		mHeight				= mTextureHeight;
		mTextureRect.left   = 0;
		mTextureRect.top	= 0;
		mTextureRect.right	= mTextureRect.left + mWidth;
		mTextureRect.bottom	= mTextureRect.top  + mHeight;

		mEndFrame = mStartFrame = 0;	// don't use animations when the textureRect is invalid
	} else {
		mWidth				= textureRect->right - textureRect->left;
		mHeight				= textureRect->bottom - textureRect->top;
		mTextureRect.left   = textureRect->left;
		mTextureRect.top	= textureRect->top;
		mTextureRect.right	= textureRect->right;
		mTextureRect.bottom	= textureRect->bottom;
	}
	mRelativeCenter = RIVec2(mWidth / 2.0f, mHeight / 2.0f) * mScale;
	mTexture		= tex;
	mbAlpha			= bAlpha;
}



void RISprite::SetTextureRect(U32 left, U32 top, U32 right, U32 bottom)
{
	if(!mbInitialized) return;

	if((right - left) <= 0 || (bottom - top) <= 0) {
		mWidth				= mTextureWidth;
		mHeight				= mTextureHeight;
		mTextureRect.left   = 0;
		mTextureRect.top	= 0;
		mTextureRect.right	= mTextureRect.left + mWidth;
		mTextureRect.bottom	= mTextureRect.top  + mHeight;
	} else {
		mWidth				= right - left;
		mHeight				= bottom - top;
		mTextureRect.left   = left;
		mTextureRect.top	= top;
		mTextureRect.right	= right;
		mTextureRect.bottom	= bottom;
	}
	mRelativeCenter = RIVec2(mWidth / 2.0f, mHeight / 2.0f) * mScale;
}


void RISprite::VOnDraw() {
	if(!mbInitialized || !mbVisible) return;
	assert(mpRI);

	float s1 = (float)mTextureRect.left / mTextureWidth;
	float t1 = (float)mTextureRect.top / mTextureHeight;
	float s2 = (float)mTextureRect.right / mTextureWidth;
	float t2 = (float)mTextureRect.bottom / mTextureHeight;

	float cosRotation = cos(mfRotation);
	float sinRotation = sin(mfRotation);

	RIVec4 topleft((-cosRotation*mRelativeCenter.x + sinRotation*mRelativeCenter.y) + GetCenter().x,
				   (-sinRotation*mRelativeCenter.x - cosRotation*mRelativeCenter.y) + GetCenter().y,
					s1, t1);
	RIVec4 topright((cosRotation*mRelativeCenter.x + sinRotation*mRelativeCenter.y) + GetCenter().x,
					(sinRotation*mRelativeCenter.x - cosRotation*mRelativeCenter.y) + GetCenter().y,
					s2, t1);
	RIVec4 botright((cosRotation*mRelativeCenter.x - sinRotation*mRelativeCenter.y) + GetCenter().x,
					(sinRotation*mRelativeCenter.x + cosRotation*mRelativeCenter.y) + GetCenter().y,
					s2, t2);
	RIVec4 botleft((-cosRotation*mRelativeCenter.x - sinRotation*mRelativeCenter.y) + GetCenter().x,
				   (-sinRotation*mRelativeCenter.x + cosRotation*mRelativeCenter.y) + GetCenter().y,
					s1, t2);

	mpRI->VDrawPic(topleft,
				   topright,
				   botright,
				   botleft,
				   mTexture, RIGUIORD_MID, mbAlpha);
}

void RISprite::VOnUpdate( float elapsedTime )
{
	if(!mbInitialized) return;
	
	if(mEndFrame - mStartFrame > 0 || mbAnimationComplete) {         // if animated sprite
		mAnimationTimer += elapsedTime;
		if(mAnimationTimer > mTimePerFrame) {
			mAnimationTimer -= mTimePerFrame;
			mCurrentFrame++;
			if(mCurrentFrame < mStartFrame || mCurrentFrame > mEndFrame) {
				if(mbLoop) mCurrentFrame = mStartFrame;
				else {
					mCurrentFrame = mEndFrame;
					mbAnimationComplete = true;
				}
			}

			SetRect();
		}
	}

}

void RISprite::SetRect()
{
	mTextureRect.left   = (mCurrentFrame % mNumColumns) * mWidth;
	mTextureRect.right  = mTextureRect.left + mWidth;
	mTextureRect.top    = (mCurrentFrame / mNumColumns) * mHeight;
	mTextureRect.bottom = mTextureRect.top + mHeight;
}

void RISprite::SetLoop( bool bLoop )
{
	if(bLoop) {
		mbLoop = true;
		mbAnimationComplete = false;
	} else mbLoop = false;
}

void RISprite::SetCurrentFrame( U32 frame )
{
	if(!mbInitialized) return;

	mCurrentFrame = frame;
	mbAnimationComplete = false;
	SetRect();
}

void RISprite::RecalcDependenciesOnScale()
{
	mRelativeCenter *= mScale;
}
