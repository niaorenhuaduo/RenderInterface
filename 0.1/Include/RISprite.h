#pragma once

#include "RIRenderInterface.h"

// ================================================
// TODO
// -OnDraw optimieren:
// sin/cos nur bei rotation erforderlich
// -mipmap bug beheben:
// sprites werden falsch dargestellt wenn sie verkleinert
// dargestellt werden und ihre Textur keine Mipmaps besitzt
// -OnDraw flipVertical und flipHorizontal implementieren
// ================================================

class RISprite {
public:
	RISprite();
	virtual ~RISprite();

	// static Sprite
	virtual void VOnInit(RIRenderInterface *pRI, HTexture texture, const RECT* textureRect = NULL, bool bAlpha = false);	
	// animated Sprite
	virtual void VOnInit(RIRenderInterface *pRI, HTexture texture, U32 twidth, U32 theight, U32 columns, U32 startFrame, U32 endFrame, double timePerFrame, bool bAlpha );

	virtual void VOnUpdate(float elapsedTime);
	virtual void VOnDraw();

	// Set
	void SetX(float x)					{ mPosition.x = x; }
	void SetY(float y)					{ mPosition.y = y; }
	void SetPosition(float x, float y)  { mPosition.Set(x, y); }
	void SetPosition(const RIVec2 &p)	{ mPosition = p; }
	void SetWidthHeight(U32 width, U32 height) { SetScale((float)width / (float)mWidth, (float)height / (float)mHeight); }

	void SetVisibility(bool visible)	{ mbVisible = visible; }
	void SetRotationRad(float fRot)		{ mfRotation = fRot; }
	void AddRotationRad(float fRot)		{ mfRotation += fRot; }
	void SetRotationDegrees(float fDeg)	{ mfRotation = ((float)M_PI/180.0f)*fDeg; }
	void AddRotationDegrees(float fDeg) { mfRotation += ((float)M_PI/180.0f)*fDeg; }
	void SetScale(float fX, float fY)	{ mScale.Set(fX, fY); mRelativeCenter = RIVec2((float)mWidth/2.0f, (float)mHeight/2.0f) * mScale; }
	void SetScale(float fScale)			{ SetScale(fScale, fScale); }
	void SetFlipVertical(bool bFlip)	{ mbFlipVertical = bFlip; }
	void SetFlipHorizontal(bool bFlip)	{ mbFlipHorizontal = bFlip; }

	void SetAlpha(bool bAlpha)			{ mbAlpha = bAlpha; }
	void SetRect();		// Selects the right image depending on the current frame
	void SetTextureRect(U32 left, U32 top, U32 right, U32 bottom);
	void SetTexture(HTexture tex, const RECT *textureRect, bool bAlpha);

	//void	Set
	void SetTimePerFrame(double delay)	{ mTimePerFrame = delay; }
	void SetFrames(U32 start, U32 end)	{ mStartFrame = start; mEndFrame = end; }
	void SetAnimationComplete(bool x)	{ mbAnimationComplete = x; }
	void SetCurrentFrame(U32 frame);
	void SetLoop(bool bLoop);

	// Get
	RIVec2 GetPosition() const		{ return mPosition; }
	float  GetX() const				{ return GetPosition().x; }
	float  GetY() const				{ return GetPosition().y; }
	RIVec2 GetRelativeCenter() const{ return mRelativeCenter; }
	RIVec2 GetCenter() const		{ return GetPosition() + GetRelativeCenter(); }
	float  GetWidth() const			{ return mWidth * mScale.x; }
	float  GetHeight() const		{ return mHeight * mScale.y; }

	bool	GetVisibility() const	{ return mbVisible; }
	float	GetRotationRad() const	{ return mfRotation; }
	float   GetRotationDegrees() const { return (mfRotation*180.0f)/(float)M_PI; }
	float	GetScaleX()	const		{ return mScale.x; }
	float	GetScaleY()	const		{ return mScale.y; }

	const RECT& GetTextureRect() const { return mTextureRect; }
	HTexture	GetTexture() const 	{ return mTexture; }
	bool		GetAlpha() const	{ return mbAlpha; }

	double GetTimePerFrame() const	{ return mTimePerFrame; }

protected:
	RIRenderInterface *mpRI;
	bool mbInitialized;

	// Position/Translation
	RIVec2      mPosition;				// position of the topleft corner
	RIVec2		mRelativeCenter;		// scaled center relative to position
	U32			mWidth, mHeight;		// not scaled width/height of the sprite
	float		mfRotation;				// in Radians
	RIVec2		mScale;
	bool		mbVisible;

	// Texture
	HTexture	mTexture;
	U32			mTextureWidth, mTextureHeight;
	RECT		mTextureRect;
	bool		mbAlpha;
	bool		mbFlipHorizontal;
	bool		mbFlipVertical;

	// Animation
	U32			mStartFrame;			// starting with 0
	U32			mEndFrame;
	U32			mNumColumns;			
	U32			mCurrentFrame;
	double		mTimePerFrame;
	double		mAnimationTimer;
	bool		mbLoop;
	bool		mbAnimationComplete;

private:
	void ResetPositionAnimation();
	void RecalcDependenciesOnScale();
};