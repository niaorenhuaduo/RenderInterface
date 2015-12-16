
#pragma once


/********************************************************************/
// INCLUDES
/********************************************************************/
#include "Vec2.h"
#include "Matrix2D.h"
#include "Rectangle.h"




namespace ri 
{

////////////////////////////////////////////////////////////
/// \brief Describes what part of the world should be seen on the screen. Only one camera can be active at the same time.
///
////////////////////////////////////////////////////////////
class RI_EXPORT Camera : private UnCopyable {
public:

	////////////////////////////////////////////////////////////
	/// \brief Creates default camera. This will project the virtual screen on the complete render target
	///
	/// > CenterPosition (0/0)
	/// > CameraSize (VIRTUALXRESOLUTION / VIRTUALYRESOLUTION)
	/// > Viewport (0.0f, 0.0f, 1.0f, 1.0f)
	///
	////////////////////////////////////////////////////////////
	Camera();

	////////////////////////////////////////////////////////////
	/// \brief Camera with a specific size and position. 
	///
	/// > Viewport (0.0f, 0.0f, 1.0f, 1.0f)
	///
	////////////////////////////////////////////////////////////
	Camera(const Vec2 &centerPos, const Vec2 &size);

	////////////////////////////////////////////////////////////
	/// \brief Sets the center of the camera
	///
	////////////////////////////////////////////////////////////
	void SetCenter(float centerX, float centerY);
	void SetCenter(const Vec2 &centerPos);
	void AddToCenter(float x, float y);
	void AddToCenter(const Vec2 &translation);

	////////////////////////////////////////////////////////////
	/// \brief Sets the size of the camera in wold coordinates
	///
	////////////////////////////////////////////////////////////
	void SetSize(float x, float y);
	void SetSize(const Vec2 &size);

	////////////////////////////////////////////////////////////
	/// \brief Specify on what part of the backbuffer the Scene should be presented
	///
	////////////////////////////////////////////////////////////
	void SetViewport(const FloatRect &vp);
	void SetViewport(float left, float top, float right, float bottom);

	// Getter
	const Vec2&			GetCenterPosition() const	{ return mCenterPos; }
	const Vec2&			GetSize() const				{ return mSize; }
	const FloatRect&	GetViewport() const			{ return mViewport; }

	void ActivateCamera();

protected:
	Vec2		mCenterPos;			// Position of the Camera in the world
	Vec2		mSize;				// Size of the space in the world(view) that gets projected onto the viewport
	FloatRect	mViewport;			// Part of the rendertarget
};



} // namespace ri