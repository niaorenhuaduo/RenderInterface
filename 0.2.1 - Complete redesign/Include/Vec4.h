#pragma once

/********************************************************************/
// INCLUDES
/********************************************************************/
#include "RI.h"


// TODO: Compare with epsilon to remove precision errors



namespace ri
{

////////////////////////////////////////////////////////////
/// \brief Four dimensional vector
///
////////////////////////////////////////////////////////////
class RI_EXPORT Vec4 {
public:
	Vec4() { Zero(); }
	explicit Vec4(float a) { Set(a, a, a, a); }
	explicit Vec4(float x, float y, float z, float w) { Set(x, y, z, w); }

	void Set(float x, float y, float z, float w);
	void Zero();

	float Dot(const Vec4 &other) const;

	// operators
	float	operator[](const U32 index) const;
	float&	operator[](const U32 index);
	Vec4    operator-() const;	// Negate

	Vec4  operator+(const Vec4 &rhs) const;
	Vec4  operator-(const Vec4 &rhs) const;
	Vec4  operator*(const float a) const;
	Vec4  operator/(const float a) const;
	Vec4  operator*(const Vec4 &rhs) const;		
	Vec4& operator+=(const Vec4 &rhs);
	Vec4& operator-=(const Vec4 &rhs);
	Vec4& operator*=(const float a);
	Vec4& operator/=(const float a);
	Vec4& operator*=(const Vec4 &rhs);
	Vec4& operator/=(const Vec4 &rhs);

	bool	Compare(const Vec4 &a) const;											// TODO compare with epsilon																		
	bool	operator==(const Vec4 &rhs) const;	// exact compare!
	bool	operator!=(const Vec4 &rhs) const;

	// Data
	float x, y, z, w;
};


// implementation
#include "Vec4.inl"



} // namespace ri