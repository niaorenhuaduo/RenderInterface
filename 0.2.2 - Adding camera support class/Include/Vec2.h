#pragma once

/********************************************************************/
// INCLUDES
/********************************************************************/
#include "RI.h"


// TODO: Compare with epsilon to remove precision errors



namespace ri
{

////////////////////////////////////////////////////////////
/// \brief Two dimensional vector
///
////////////////////////////////////////////////////////////
class RI_EXPORT Vec2 {
public:
	float x, y;

	Vec2() { Zero(); }
	explicit Vec2(float a) { Set(a, a); }
	explicit Vec2(float x, float y) { Set(x, y); }

	void Set(float x, float y);
	void Zero();

	float Dot(const Vec2 &other) const;

	float ToAngle() { return atan2(y, x); }
	float GetLength() { return sqrt(x*x + y*y); }
	float GetLengthSquared() { return x*x + y*y; }

	// operators
	float	operator[](const U32 index) const;
	float&	operator[](const U32 index);
	Vec2  operator-() const;	// Negate

	Vec2  operator+(const Vec2 &rhs) const;
	Vec2  operator-(const Vec2 &rhs) const;
	Vec2  operator*(const float a) const;
	Vec2  operator/(const float a) const;
	Vec2  operator*(const Vec2 &rhs) const;		
	Vec2& operator+=(const Vec2 &rhs);
	Vec2& operator-=(const Vec2 &rhs);
	Vec2& operator*=(const float a);
	Vec2& operator*=(const Vec2 &rhs);
	Vec2& operator/=(const float a);
	Vec2& operator/=(const Vec2 &rhs);

	bool	Compare(const Vec2 &a) const;											// TODO compare with epsilon																		
	bool	operator==(const Vec2 &rhs) const;	// exact compare!
	bool	operator!=(const Vec2 &rhs) const;
};


// implementation
#include "Vec2.inl"

} // namespace ri