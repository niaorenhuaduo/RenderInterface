#include "..\Include\Camera.h"
#include "..\Include\IRenderSystem.h"
#include "..\Include\ServiceLocator.h"


namespace 
{
	static ri::Camera *pActiveCamera = NULL;
}

namespace ri
{


void Camera::ActivateCamera() {
	pActiveCamera = this;

	ServiceLocator::RenderSystem()._SetCameraPosition(mCenterPos);
	ServiceLocator::RenderSystem()._SetCameraSize(static_cast<U32>(mSize.x), static_cast<U32>(mSize.y));
	ServiceLocator::RenderSystem()._SetViewport(mViewport);

	ServiceLocator::RenderSystem()._CalcAndSetViewMatrix();
	ServiceLocator::RenderSystem()._CalcAndSetProjMatrix();
}


Camera::Camera() : 
mCenterPos(static_cast<float>(VIRTUALXRESOLUTION) / 2.0f, static_cast<float>(VIRTUALYRESOLUTION) / 2.0f), 
mSize(static_cast<float>(VIRTUALXRESOLUTION), static_cast<float>(VIRTUALYRESOLUTION)), 
mViewport(0.0f, 0.0f, 1.0f, 1.0f)
{}


Camera::Camera( const Vec2 &centerPos, const Vec2 &size ) : 
mCenterPos(centerPos),
mSize(size),
mViewport(0.0f, 0.0f, 1.0f, 1.0f)
{
	if(mSize.x <= 0 || mSize.y <= 0) {
		mSize.x = VIRTUALXRESOLUTION;
		mSize.y = VIRTUALYRESOLUTION;
	}
}




void Camera::SetCenter( float centerX, float centerY )
{
	mCenterPos.x = centerX;
	mCenterPos.y = centerY;

	if(pActiveCamera == this) {
		ServiceLocator::RenderSystem()._SetCameraPosition(mCenterPos);
		ServiceLocator::RenderSystem()._CalcAndSetViewMatrix();
	}
}

void Camera::SetCenter( const Vec2 &centerPos )
{
	SetCenter(centerPos.x, centerPos.y);
}

void Camera::AddToCenter( float x, float y )
{
	mCenterPos.x += x;
	mCenterPos.y += y;

	if(pActiveCamera == this) {
		ServiceLocator::RenderSystem()._SetCameraPosition(mCenterPos);
		ServiceLocator::RenderSystem()._CalcAndSetViewMatrix();
	}
}

void Camera::AddToCenter( const Vec2 &translation )
{
	AddToCenter(translation.x, translation.y);
}

void Camera::SetSize( float x, float y )
{
	mSize.x = x;
	mSize.y = y;

	if(x <= 0 || y <= 0) {
		mSize.x = VIRTUALXRESOLUTION;
		mSize.y = VIRTUALYRESOLUTION;
	} 

	if(pActiveCamera == this) {
		ServiceLocator::RenderSystem()._SetCameraSize(static_cast<U32>(mSize.x), static_cast<U32>(mSize.y));
		ServiceLocator::RenderSystem()._CalcAndSetProjMatrix();
	}
}

void Camera::SetSize( const Vec2 &size )
{
	SetSize(size.x, size.y);
}

void Camera::SetViewport( float left, float top, float right, float bottom )
{
	assert(left >= 0.0f && right <= 1.0f && top >= 0.0f && bottom <= 1.0f &&
		   left < right && top < bottom && "RICamera::SetViewport: forbidden values!");

	mViewport.left = left;
	mViewport.width = right - left;
	mViewport.top = top;
	mViewport.height = bottom - top;

	if(pActiveCamera == this) {
		ServiceLocator::RenderSystem()._SetViewport(mViewport);
		ServiceLocator::RenderSystem()._CalcAndSetProjMatrix();
	}
}

void Camera::SetViewport( const FloatRect &vp )
{
	SetViewport(vp.left, vp.top, vp.left+vp.width, vp.top+vp.height);
}






} // namespace ri