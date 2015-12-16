#pragma once

class RIVec2;


////////////////////////////////////
// Row-Major Matrix using Row-Vectors
////////////////////////////////////
class RIMatrix2D {
public:
	RIMatrix2D();
	RIMatrix2D(float f11, float f12, float f13,
			   float f21, float f22, float f23,
			   float f31, float f32, float f33);

	void Identity();

	// add the transformation to this matrix
	RIMatrix2D& Translate(float x, float y);
	RIMatrix2D& Translate(const RIVec2 &point);
	RIMatrix2D& Rotate(float angle);				// in degrees
	RIMatrix2D& Scale(float x, float y);
	RIMatrix2D& Scale(const RIVec2 &factors);

	RIMatrix2D GetInverse() const;

	// operators
	RIMatrix2D& operator*=(const RIMatrix2D &other);

	// Copy constr/assign
	RIMatrix2D(const RIMatrix2D &other);
	RIMatrix2D& operator=(const RIMatrix2D &rhs);

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

	friend RIMatrix2D operator*(const RIMatrix2D &left, const RIMatrix2D &right);
	friend RIVec2	  operator*(const RIVec2 &left,	   const RIMatrix2D &right);
};

////////////////////////////////////
// Operators
////////////////////////////////////
RIMatrix2D  operator* (const RIMatrix2D &left, const RIMatrix2D &right);
RIVec2		operator* (const RIVec2 &left,	   const RIMatrix2D &right);



////////////////////////////////////////////////////////////
/// Implementation
////////////////////////////////////////////////////////////
inline RIMatrix2D::RIMatrix2D() {
	Identity();
}

inline RIMatrix2D::RIMatrix2D(float f11, float f12, float f13,
							  float f21, float f22, float f23,
							  float f31, float f32, float f33) {
	Set(f11, f12, f13,
		f21, f22, f23,
		f31, f32, f33);
}

inline RIMatrix2D::RIMatrix2D( const RIMatrix2D &other ) {
	Set(other._11, other._12, other._14,
		other._21, other._22, other._24,
		other._41, other._42, other._44);
}


inline RIMatrix2D& RIMatrix2D::operator=( const RIMatrix2D &rhs ){
	Set(rhs._11, rhs._12, rhs._14,
		rhs._21, rhs._22, rhs._24,
		rhs._41, rhs._42, rhs._44);

	return (*this);
}

inline void RIMatrix2D::Identity() {
	Set(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f);
}

inline void RIMatrix2D::Set(float f11, float f12, float f13,
							float f21, float f22, float f23,
							float f31, float f32, float f33) {
	_11 = f11; _12 = f12; _14 = f13;
	_21 = f21; _22 = f22; _24 = f23;
	_41 = f31; _42 = f32; _44 = f33;

	_13 = _23 = _33 = _43 = 0.0f;
	_31 = _32 = _34		  = 0.0f;
}

inline RIMatrix2D& RIMatrix2D::Translate( float x, float y ) {
	RIMatrix2D translate(1.0f, 0.0f, 0.0f,
						 0.0f, 1.0f, 0.0f,
						 x   , y   , 1.0f);

	return (*this) *= translate;
}

inline RIMatrix2D& RIMatrix2D::Translate( const RIVec2 &point ) {
	return Translate(point.x, point.y);
}

inline RIMatrix2D& RIMatrix2D::Rotate( float angle ) {
	float rad = RIDegreeToRadians(angle);
	float c = cosf(rad);
	float s = sinf(rad);

	RIMatrix2D rotation(c   , s   , 0.0f,
						-s  , c   , 0.0f,
						0.0f, 0.0f, 1.0f);

	return (*this) *= rotation;
}

inline RIMatrix2D& RIMatrix2D::Scale( float x, float y ) {
	RIMatrix2D scaling(x   , 0.0f, 0.0f,
					   0.0f, y   , 0.0f,
					   0.0f, 0.0f, 1.0f);
	
	return (*this) *= scaling;
}

inline RIMatrix2D& RIMatrix2D::Scale( const RIVec2 &factors ) {
	return Scale(factors.x, factors.y);
}


// operators
inline RIMatrix2D  operator* (const RIMatrix2D &left, const RIMatrix2D &right) {
	return RIMatrix2D(left._11*right._11 + left._12*right._21 + left._14*right._41,
						 left._11*right._12 + left._12*right._22 + left._14*right._42,
						 left._11*right._14 + left._12*right._24 + left._14*right._44,
						 left._21*right._11 + left._22*right._21 + left._24*right._41,
						 left._21*right._12 + left._22*right._22 + left._24*right._42,
						 left._21*right._14 + left._22*right._24 + left._24*right._44,
						 left._41*right._11 + left._42*right._21 + left._44*right._41,
						 left._41*right._12 + left._42*right._22 + left._44*right._42,
						 left._41*right._14 + left._42*right._24 + left._44*right._44);						 
}

inline RIVec2 operator*(const RIVec2 &left, const RIMatrix2D &right) {
	return RIVec2(left.x*right._11 + left.y*right._21 + right._41,
				  left.x*right._12 + left.y*right._22 + right._42);
}


inline RIMatrix2D& RIMatrix2D::operator*=(const RIMatrix2D &other) {
	(*this) = (*this) * other;
	return (*this);
}

inline RIMatrix2D RIMatrix2D::GetInverse() const
{
	float det = _11 * (_22*_44 - _24*_42) -
				_12 * (_21*_44 - _24*_41) +
				_14 * (_21*_42 - _22*_41);

	if(det != 0) {
		return RIMatrix2D();
	} else return RIMatrix2D();
}

