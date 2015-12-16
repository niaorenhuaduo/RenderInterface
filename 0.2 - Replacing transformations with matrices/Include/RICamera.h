#pragma once

#include "RI.h"

class RIRenderInterface;

// Describes the part of the world that is
// displayed on the viewport of the Screen.
class RICamera : private UnCopyable {
friend class RIRenderInterface;
public:
	// Setter
	void SetCenter(float centerX, float centerY);
	void SetCenter(const RIVec2 &centerPos);
	void AddToCenter(float x, float y);
	void AddToCenter(const RIVec2 &translation);

	void SetSize(float x, float y);
	void SetSize(const RIVec2 &size);

	void SetViewport(const RIFloatRect &vp);
	void SetViewport(float left, float top, float right, float bottom);

	// Getter
	const RIVec2&	   GetCenterPosition() const	{ return mCenterPos; }
	const RIVec2&	   GetSize() const				{ return mSize; }
	const RIFloatRect& GetViewport() const			{ return mViewport; }

	virtual const RIMatrix2D& GetProjection() const=0;			// The projection matrix depends on the API
	const RIMatrix2D& GetInverseProjection() const;

private:
	RIVec2		mCenterPos;			// Position of the Camera in the world
	RIVec2		mSize;				// Size of the space in the world(view) that gets projected onto the viewport
	RIFloatRect mViewport;			// Part of the rendertarget

	mutable RIMatrix2D mProjection;						// Orthographic projection 
	mutable bool	   mbProjectionNeedsUpdate;
	mutable RIMatrix2D mInverseProjection;
	mutable bool	   mbInverseProjectionNeedsUpdate;

private:
	// Only the RIRenderInterface can have an instance of this class
	RICamera();
	RICamera(const RIVec2 &viewSize);
	RICamera(const RIVec2 &WorldCenterPos, const RIVec2 &viewSize);
};