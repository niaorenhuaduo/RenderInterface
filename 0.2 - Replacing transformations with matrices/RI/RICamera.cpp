#include "..\Include\RICamera.h"
#include "..\Include\RIRenderInterface.h"


RICamera::RICamera() : mCenterPos(0.0f), mSize((float)RIVIRTUALXRESOLUTION, (float)RIVIRTUALYRESOLUTION), mViewport(0.0f, 0.0f, 1.0f, 1.0f), 
	mProjection(), mInverseProjection(), mbProjectionNeedsUpdate(true), mbInverseProjectionNeedsUpdate(true)  {}

RICamera::RICamera(const RIVec2 &viewSize ) : mCenterPos(0.0f), mSize(viewSize), mViewport(0.0f, 0.0f, 1.0f, 1.0f), mProjection(), mInverseProjection(),
	mbProjectionNeedsUpdate(true), mbInverseProjectionNeedsUpdate(true) {
	if(viewSize.x <= 0 || viewSize.y <= 0) {
		mSize.x = RIVIRTUALXRESOLUTION;
		mSize.y = RIVIRTUALYRESOLUTION;
	}
}

RICamera::RICamera(const RIVec2 &centerPos, const RIVec2 &viewSize ) : mCenterPos(centerPos), mSize(viewSize), mViewport(0.0f, 0.0f, 1.0f, 1.0f), mProjection(), mInverseProjection(),
	mbProjectionNeedsUpdate(true), mbInverseProjectionNeedsUpdate(true) {
	if(viewSize.x <= 0 || viewSize.y <= 0) {
		mSize.x = RIVIRTUALXRESOLUTION;
		mSize.y = RIVIRTUALYRESOLUTION;
	}
}

void RICamera::SetCenter( float centerX, float centerY )
{
	mCenterPos.x = centerX;
	mCenterPos.y = centerY;

	mbProjectionNeedsUpdate			= true;
	mbInverseProjectionNeedsUpdate	= true;
}

void RICamera::SetCenter( const RIVec2 &centerPos )
{
	SetCenter(centerPos.x, centerPos.y);
}

void RICamera::AddToCenter( float x, float y )
{
	mCenterPos.x += x;
	mCenterPos.y += y;

	mbProjectionNeedsUpdate			= true;
	mbInverseProjectionNeedsUpdate	= true;
}

void RICamera::AddToCenter( const RIVec2 &translation )
{
	AddToCenter(translation.x, translation.y);
}

void RICamera::SetSize( float x, float y )
{
	mSize.x = x;
	mSize.y = y;

	if(x <= 0 || y <= 0) {
		mSize.x = RIVIRTUALXRESOLUTION;
		mSize.y = RIVIRTUALYRESOLUTION;
	} 

	mbProjectionNeedsUpdate			= true;
	mbInverseProjectionNeedsUpdate	= true;
}

void RICamera::SetSize( const RIVec2 &size )
{
	SetSize(size.x, size.y);
}

void RICamera::SetViewport( float left, float top, float right, float bottom )
{
	assert(left >= 0.0f && right <= 1.0f && top >= 0.0f && bottom <= 1.0f &&
		   left < right && top < bottom && "RICamera::SetViewport: forbidden values!");

	mViewport.left = left;
	mViewport.width = right - left;
	mViewport.top = top;
	mViewport.height = bottom - top;
}

void RICamera::SetViewport( const RIFloatRect &vp )
{
	SetViewport(vp.left, vp.top, vp.left+vp.width, vp.top+vp.height);
}


const RIMatrix2D& RICamera::GetInverseProjection() const
{
	if(mbInverseProjectionNeedsUpdate) {
		mInverseProjection = GetProjection().GetInverse();
		
		mbInverseProjectionNeedsUpdate = false;
	}

	return mInverseProjection;
}