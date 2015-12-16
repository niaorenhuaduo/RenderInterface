#pragma once

#include "..\Transformable.h"
#include "..\IRenderSystem.h"
#include "..\Vertex.h"
#include "Drawable.h"

// ================================================
// 
// -GetGlobalBoundingBox()
// -negative skalierung macht das sprite unsichtbar weil der normal vektor in die falsche richtung zeigt
// ================================================

namespace ri
{


////////////////////////////////////////////////////////////
/// \brief A transformable rectangle with a texture, alpha blending and optional animation
///
////////////////////////////////////////////////////////////
class RI_EXPORT Sprite : public Drawable, public Transformable {
public:
	Sprite();
	Sprite(HTexture tex, bool bAlpha = false);
	Sprite(HTexture tex, const IntRect &textureRect, bool bAlpha = false);
	Sprite(HTexture tex, U32 frameWidth, U32 frameHeight, U32 numColumns, U32 startFrame, U32 endFrame, double frameDelay, bool bLooped = false, bool bAlpha = false);
	virtual ~Sprite();

	// just copy everything
/*	Sprite(const Sprite &rhs);
	Sprite& operator=(const Sprite &rhs);*/

	void OnUpdate(double fElapsedTime);
	void OnDraw();

	// Set
	void SetTexture(HTexture tex);					// Will disable animation
	void SetTextureRect(const IntRect &texRect);	// Will disable animation
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
	const IntRect& GetTextureRect() const { return mTextureRect; }

	bool GetAlpha()	const				{ return mbAlpha; }
	bool GetFlippingHorizontal() const	{ return mbFlipHorizontal; }
	bool GetFlippingVertical() const	{ return mbFlipVertical; }
	bool GetVisible() const				{ return mbVisible; }

	bool   GetAnimated() const			{ return mbAnimated; }
	double GetFrameDelay() const		{ return mAnimation.GetFrameDelay(); }
	bool   GetLooped() const			{ return mAnimation.GetLooped(); }
	U32	   GetCurrentFrame() const		{ return mAnimation.GetCurrentFrame(); }
	bool   GetAnimationComplete() const { return mAnimation.GetAnimationComplete(); }

	////////////////////////////////////////////////////////////
	/// \brief Returns a bounding box that encloses the Sprite in local space
	///
	////////////////////////////////////////////////////////////
	FloatRect			GetLocalBoundingBox() const;

	////////////////////////////////////////////////////////////
	/// \brief Returns an AABB that encloses the Sprite after transformation
	///
	////////////////////////////////////////////////////////////
	FloatRect			GetGlobalBoundingBox() const;

private:
	class RI_EXPORT Animation {
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
	Vertex2D    mVertices[4];
	IntRect     mTextureRect;
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


} // namespace ri

