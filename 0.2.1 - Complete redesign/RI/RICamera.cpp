/*
#include "..\Include\RICamera.h"
#include "..\Include\RIRenderInterface.h"



void RICamera::SetCenter( float centerX, float centerY )
{
	mCenterPos.x = centerX;
	mCenterPos.y = centerY;

	mbProjectionNeedsUpdate			= true;
	mbInverseProjectionNeedsUpdate	= true;
}

void RICamera::SetCenter( const Vec2 &centerPos )
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

void RICamera::AddToCenter( const Vec2 &translation )
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

void RICamera::SetSize( const Vec2 &size )
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
}*/