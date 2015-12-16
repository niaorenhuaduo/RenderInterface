
inline Matrix2D::Matrix2D() {
	Identity();
}

inline Matrix2D::Matrix2D(float f11, float f12, float f13,
						  float f21, float f22, float f23,
						  float f31, float f32, float f33) {
	Set(f11, f12, f13,
	    f21, f22, f23,
		f31, f32, f33);
}


inline void Matrix2D::Identity() {
	Set(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f);
}

inline void Matrix2D::Set(float f11, float f12, float f13,
							float f21, float f22, float f23,
							float f31, float f32, float f33) {
								_11 = f11; _12 = f12; _14 = f13;
								_21 = f21; _22 = f22; _24 = f23;
								_41 = f31; _42 = f32; _44 = f33;

								_13 = _23 = _33 = _43 = 0.0f;
								_31 = _32 = _34		  = 0.0f;
}

inline Matrix2D& Matrix2D::Translate( float x, float y ) {
	Matrix2D translate(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		x   , y   , 1.0f);

	return (*this) *= translate;
}

inline Matrix2D& Matrix2D::Translate( const Vec2 &point ) {
	return Translate(point.x, point.y);
}

inline Matrix2D& Matrix2D::Rotate( float angle ) {
	float rad = DegreeToRadians(angle);
	float c = cosf(rad);
	float s = sinf(rad);

	Matrix2D rotation(c   , s   , 0.0f,
		-s  , c   , 0.0f,
		0.0f, 0.0f, 1.0f);

	return (*this) *= rotation;
}

inline Matrix2D& Matrix2D::Scale( float x, float y ) {
	Matrix2D scaling(x   , 0.0f, 0.0f,
		0.0f, y   , 0.0f,
		0.0f, 0.0f, 1.0f);

	return (*this) *= scaling;
}

inline Matrix2D& Matrix2D::Scale( const Vec2 &factors ) {
	return Scale(factors.x, factors.y);
}


// operators
inline Matrix2D  operator* (const Matrix2D &left, const Matrix2D &right) {
	return Matrix2D(left._11*right._11 + left._12*right._21 + left._14*right._41,
		left._11*right._12 + left._12*right._22 + left._14*right._42,
		left._11*right._14 + left._12*right._24 + left._14*right._44,
		left._21*right._11 + left._22*right._21 + left._24*right._41,
		left._21*right._12 + left._22*right._22 + left._24*right._42,
		left._21*right._14 + left._22*right._24 + left._24*right._44,
		left._41*right._11 + left._42*right._21 + left._44*right._41,
		left._41*right._12 + left._42*right._22 + left._44*right._42,
		left._41*right._14 + left._42*right._24 + left._44*right._44);						 
}

inline Vec2 operator*(const Vec2 &left, const Matrix2D &right) {
	return Vec2(left.x*right._11 + left.y*right._21 + right._41,
		left.x*right._12 + left.y*right._22 + right._42);
}


inline Matrix2D& Matrix2D::operator*=(const Matrix2D &other) {
	(*this) = (*this) * other;
	return (*this);
}

inline Matrix2D Matrix2D::GetInverse() const
{
	float det = _11 * (_22*_44 - _24*_42) -
		_12 * (_21*_44 - _24*_41) +
		_14 * (_21*_42 - _22*_41);

	if(det != 0) {
		return Matrix2D();
	} else return Matrix2D();
}

inline FloatRect Matrix2D::TransformRect(const FloatRect &rect) const {
	// calculate the transformed points
	const Vec2 points[] = {
		Vec2(rect.left,					rect.top) * (*this),
		Vec2(rect.left + rect.width,	rect.top) * (*this),
		Vec2(rect.left,					rect.top + rect.height) * (*this),
		Vec2(rect.left + rect.width,	rect.top + rect.height) * (*this)
	};

	// return a AABB that encloses this points
	float left = points[0].x;
	float top = points[0].y;
	float right = points[0].x;
	float bottom = points[0].y;
	for (U32 i = 1; i < 4; ++i)
	{
		if      (points[i].x < left)   left = points[i].x;
		else if (points[i].x > right)  right = points[i].x;
		if      (points[i].y < top)    top = points[i].y;
		else if (points[i].y > bottom) bottom = points[i].y;
	}

	return FloatRect(left, top, right - left, bottom - top);
}