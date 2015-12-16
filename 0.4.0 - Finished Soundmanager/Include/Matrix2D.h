#pragma once

/********************************************************************/
// INCLUDES
/********************************************************************/
#include "Vec2.h"
#include "Rectangle.h"


#pragma warning( disable : 4201 )


namespace ri
{


////////////////////////////////////////////////////////////
/// \brief Row-Major Matrix using Row-Vectors
///
////////////////////////////////////////////////////////////
class RI_EXPORT Matrix2D {
public:
	static const Matrix2D IdentityMatrix;

public:

	////////////////////////////////////////////////////////////
	/// \brief Identity
	///
	////////////////////////////////////////////////////////////
	Matrix2D();
	Matrix2D(float f11, float f12, float f13,
		float f21, float f22, float f23,
		float f31, float f32, float f33);

	void Identity();

	////////////////////////////////////////////////////////////
	/// \brief Add the transformation to the actual matrix
	///
	////////////////////////////////////////////////////////////
	Matrix2D& Translate(float x, float y);
	Matrix2D& Translate(const Vec2 &point);
	Matrix2D& Rotate(float angle);				// in degrees
	Matrix2D& Scale(float x, float y);
	Matrix2D& Scale(const Vec2 &factors);

	FloatRect TransformRect(const FloatRect &rect) const;
	Matrix2D  GetInverse() const;

	// operators
	Matrix2D& operator*=(const Matrix2D &other);


private:
	union {
		float m[4][4];
		struct {
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
	};

	void Set(float f11, float f12, float f13,
		float f21, float f22, float f23,
		float f31, float f32, float f33);

	friend Matrix2D operator*(const Matrix2D &left, const Matrix2D &right);
	friend Vec2	    operator*(const Vec2 &left,	   const Matrix2D &right);
};



/********************************************************************/
// OPERATORS
/********************************************************************/
Matrix2D  operator* (const Matrix2D &left, const Matrix2D &right);
Vec2	  operator* (const Vec2 &left,	   const Matrix2D &right);


// implementation
#include "Matrix2D.inl"


} // namespace ri
