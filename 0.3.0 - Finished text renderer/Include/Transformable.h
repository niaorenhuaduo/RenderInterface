#pragma once

/********************************************************************/
// TODO
// Optimize GetTransformation
/********************************************************************/


/********************************************************************/
// INCLUDES
/********************************************************************/
#include "RI.h"
#include "Vec2.h"
#include "Matrix2D.h"


namespace ri
{


////////////////////////////////////////////////////////////
/// \brief Describes a two dimensional transformation and calculates it's (inverse) projection
///
////////////////////////////////////////////////////////////
class RI_EXPORT Transformable {
public:

	////////////////////////////////////////////////////////////
	/// \brief Identity Transformation
	///
	////////////////////////////////////////////////////////////
	Transformable();
	virtual ~Transformable();

	////////////////////////////////////////////////////////////
	/// \brief Sets the position of the origin in the world
	///
	////////////////////////////////////////////////////////////
	void SetPosition(float x, float y);
	void SetPosition(const Vec2 &pos);

	////////////////////////////////////////////////////////////
	/// \brief Add the translation to the current position in the world
	///
	////////////////////////////////////////////////////////////
	void AddToPosition(float x, float y);
	void AddToPosition(const Vec2 &offset);

	////////////////////////////////////////////////////////////
	/// \brief Set the origin for all the other transformations
	///
	/// The origin gets translated to the position vector
	/// The object gets rotated around the origin
	/// The object gets scaled around the origin
	///
	////////////////////////////////////////////////////////////
	void SetOrigin(float x, float y);
	void SetOrigin(const Vec2 &origin);

	////////////////////////////////////////////////////////////
	/// \brief Apply scaling for each axis
	///
	////////////////////////////////////////////////////////////
	void SetScaling(float x, float y);
	void SetScaling(const Vec2 &factors);
	void SetScaling(float factor);

	////////////////////////////////////////////////////////////
	/// \brief The scaling gets multiplied with the currend scaling
	///
	////////////////////////////////////////////////////////////
	void Scale(float x, float y);				
	void Scale(const Vec2 &factors);			
	void Scale(float factor);					

	////////////////////////////////////////////////////////////
	/// \brief Sets the rotation in degrees
	///
	////////////////////////////////////////////////////////////
	void SetRotation(float degrees);

	////////////////////////////////////////////////////////////
	/// \brief Add the rotation to the current rotation in degrees
	///
	////////////////////////////////////////////////////////////
	void Rotate(float degrees);					

	// Getter
	const Vec2& GetPosition() const	{ return mPosition; }
	const Vec2& GetOrigin() const		{ return mOrigin; }
	const Vec2& GetScaling() const	{ return mScaling; }
	float		GetRotation() const	{ return mRotation; }

	const Matrix2D& GetTransformation() const;
	const Matrix2D& GetInverseTransformation() const;

private:
	Vec2 mPosition;					// Position of the Origin
	Vec2 mOrigin;					// scale and rotate around this point; relative to the upper left corner
	Vec2 mScaling;
	float  mRotation;				// in degrees
		
	// They are mutable to ensure GetTransformation can remain const
	mutable bool		mbTransformationNeedsUpdate;
	mutable Matrix2D	mTransformation;
	mutable bool		mbInverseTransformationNeedsUpdate;
	mutable Matrix2D	mInverseTransformation;
};




} // namespace ri