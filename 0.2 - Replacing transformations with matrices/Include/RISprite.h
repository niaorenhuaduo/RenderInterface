#pragma once

#include "RITransformable.h"
#include "RIRenderInterface.h"

// ================================================
// TODO
// -GetGlobalBoundingBox()
// -negative skalierung macht das sprite unsichtbar weil der normal vektor in die falsche richtung zeigt
// ================================================

class RISprite : public RITransformable {
public:
	RISprite();
	RISprite(HTexture tex, bool bAlpha = false);
	RISprite(HTexture tex, const RIIntRect &textureRect, bool bAlpha = false);
	RISprite(HTexture tex, U32 frameWidth, U32 frameHeight, U32 numColumns, U32 startFrame, U32 endFrame, double frameDelay, bool bLooped = false, bool bAlpha = false);
	virtual ~RISprite();

	RISprite(const RISprite &rhs);
	RISprite& operator=(const RISprite &rhs);

	void OnUpdate(double fElapsedTime);
	void OnDraw(RIRenderInterface *pRI);

	// Set
	void SetTexture(HTexture tex);					// Will disable animation
	void SetTextureRect(const RIIntRect &texRect);	// Will disable animation
	void SetAnimation(U32 frameWidth, U32 frameHeight, U32 numColumns, U32 startFrame, U32 endFrame, double frameDelay, bool bLooped = false);

	void SetAlpha(bool bAlpha)				{ mbAlpha = bAlpha; }
	void SetVisibility(bool bVisible)		{ mbVisible = bVisible; }
	void SetFlippingHorizontal(bool bFlip)  { mbFlipHorizontal = bFlip; }
	void SetFlippingVertical(bool bFlip)	{ mbFlipVertical = bFlip; }	

	void SetFrameDelay(double delay)	{ mAnimation.SetFrameDelay(delay); }
	void SetFrames(U32 start, U32 end)	{ mAnimation.SetFrames(start, end); }
	void SetCurrentFrame(U32 frame)		{ mAnimation.SetCurrentFrame(frame); }
	void SetLooped(bool looped)			{ mAnimation.SetLoop(looped); }


	// Get
	HTexture		 GetTexture() const		{ return mTexture; }
	const RIIntRect& GetTextureRect() const { return mTextureRect; }

	bool GetAlpha()	const				{ return mbAlpha; }
	bool GetFlippingHorizontal() const	{ return mbFlipHorizontal; }
	bool GetFlippingVertical() const	{ return mbFlipVertical; }
	bool GetVisible() const				{ return mbVisible; }

	bool   GetAnimated() const			{ return mbAnimated; }
	double GetFrameDelay() const		{ return mAnimation.GetFrameDelay(); }
	bool   GetLooped() const			{ return mAnimation.GetLooped(); }
	U32	   GetCurrentFrame() const		{ return mAnimation.GetCurrentFrame(); }
	bool   GetAnimationComplete() const { return mAnimation.GetAnimationComplete(); }

	RIFloatRect GetLocalBoundingBox() const;	// Returns a bounding box that defines the sprite in local space
	//RIFloatRect GetGlobalBoundingBox() const;	// Returns a bounding box that covers the sprite in global space (after transformation)

private:
	struct _vertex {
		RIVec2 pos;
		RIVec2 uv;
	};

	class Animation {
	public:
		Animation();
		~Animation();

		void Init(U32 start, U32 end, double frameDelay, bool bLooped=false);
		void OnUpdate(double fElapsedTime);

		void SetFrameDelay(double delay)	{ if(delay < 0.0f) mFrameDelay = delay * -1.0; }
		void SetFrames(U32 start, U32 end)	{ if(start < end) mStartFrame = start; mEndFrame = end; mCurrentFrame = start; }
		void SetCurrentFrame(U32 frame)		{ if(frame >= mStartFrame && frame <= mEndFrame) mCurrentFrame = frame; }
		void SetLoop(bool bLooped)			{ mbLooped = bLooped; }

		U32	 GetCurrentFrame() const { return mCurrentFrame; }

		double GetFrameDelay() const		{ return mFrameDelay; }
		bool   GetLooped() const			{ return mbLooped; }
		bool   GetAnimationComplete() const	{ return (mCurrentFrame == mEndFrame && mbLooped == false); }

	private:
		U32		mStartFrame;
		U32		mEndFrame;
		U32		mCurrentFrame;
		double	mFrameDelay;
		double  mAnimTimer;
		bool	mbLooped;
	};

	// Sprite data
	HTexture	mTexture;
	U32			mTexWidth, mTexHeight;
	_vertex     mVertices[4];
	RIIntRect   mTextureRect;
	bool		mbAlpha;
	bool		mbFlipHorizontal;
	bool		mbFlipVertical;
	bool		mbVisible;

	// Animation
	Animation   mAnimation;
	bool		mbAnimated;
	U32			mNumCols;

private:
	void UpdatePositions();
	void UpdateTextureCoordinates();
	void UpdateTextureRectPositionOnAnimation(U32 frame);
	void Clear();
};




/*
#pragma once

#include "RITransformable.h"
#include "RIRenderInterface.h"

// ================================================
// TODO
// GetGlobalBoundingBox()
// -mipmap bug beheben: bug noch vorhanden?
// sprites werden falsch dargestellt wenn sie verkleinert
// dargestellt werden und ihre Textur keine Mipmaps besitzt
// -OnDraw flipVertical und flipHorizontal implementieren
// ================================================

class RISprite : public RITransformable {
public:
RISprite();
RISprite(HTexture tex, bool bAlpha = false);
RISprite(HTexture tex, const RIIntRect &textureRect, bool bAlpha = false);
virtual ~RISprite();

void OnDraw(RIRenderInterface *pRI);

// Set
void SetTexture(HTexture tex);
void SetTextureRect(const RIIntRect &texRect);

void SetAlpha(bool bAlpha)				{ mbAlpha = bAlpha; }
void SetVisibility(bool bVisible)		{ mbVisible = bVisible; }
void SetFlippingHorizontal(bool bFlip)  { mbFlipHorizontal = bFlip; }
void SetFlippingVertical(bool bFlip)	{ mbFlipVertical = bFlip; }	

// Get
HTexture		 GetTexture() const		{ return mTexture; }
const RIIntRect& GetTextureRect() const { return mTextureRect; }

bool GetAlpha()				 { return mbAlpha; }
bool GetFlippingHorizontal() { return mbFlipHorizontal; }
bool GetFlippingVertical()   { return mbFlipVertical; }
bool GetVisible()			 { return mbVisible; }

RIFloatRect GetLocalBoundingBox() const;	// Returns a bounding box that defines the sprite in local space
//RIFloatRect GetGlobalBoundingBox() const;	// Returns a bounding box that covers the sprite in global space (after transformation)

private:
struct _vertex {
RIVec2 pos;
RIVec2 uv;
};

// Sprite data
HTexture	mTexture;
U32			mTexWidth, mTexHeight;
_vertex     mVertices[4];
RIIntRect   mTextureRect;
bool		mbAlpha;
bool		mbFlipHorizontal;
bool		mbFlipVertical;
bool		mbVisible;


private:
void UpdatePositions();
void UpdateTextureCoordinates();
void Clear();
};
*/