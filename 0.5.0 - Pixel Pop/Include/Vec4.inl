inline void Vec4::Set(float x, float y, float z, float w) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

inline void Vec4::Zero() {
	x = y = z = w = 0.0f;
}

inline float Vec4::operator[](const U32 index) const {
	return (&x)[index];
}

inline float& Vec4::operator[](const U32 index) {
	return (&x)[index];
}

inline Vec4 Vec4::operator-() const {
	return Vec4(-x, -y, -z, -w);
}

inline Vec4 Vec4::operator+(const Vec4 &rhs) const {
	return Vec4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
}

inline Vec4 Vec4::operator-(const Vec4 &rhs) const {
	return Vec4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
}

inline Vec4 Vec4::operator*(const float a) const {
	return Vec4(x * a, y * a, z * a, w * a);
}

inline Vec4 Vec4::operator/(const float a) const {
	float inv = 1.0f/a;
	return Vec4(x * inv, y * inv, z * inv, w * inv);
}

inline Vec4 Vec4::operator*(const Vec4 &rhs) const {
	return Vec4(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w);
}

inline Vec4& Vec4::operator+=(const Vec4 &rhs) {
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	w += rhs.w;

	return *this;
}

inline Vec4& Vec4::operator-=(const Vec4 &rhs) {
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	w -= rhs.w;

	return *this;
}

inline Vec4& Vec4::operator*=(const float a) {
	x *= a;
	y *= a;
	z *= a;
	w *= a;

	return *this;
}

inline Vec4& Vec4::operator/=(const float a) {
	float inva = 1.0f / a;
	x *= inva;
	y *= inva;
	z *= inva;
	w *= inva;

	return *this;
}


inline Vec4& Vec4::operator*=( const Vec4 &rhs )
{
	x *= rhs.x;
	y *= rhs.y;
	z *= rhs.z;
	w *= rhs.w;

	return *this;
}


inline Vec4& Vec4::operator/=(const Vec4 &rhs) {
	x /= rhs.x;
	y /= rhs.y;
	z /= rhs.z;
	w /= rhs.w;

	return *this;
}

inline bool	Vec4::Compare(const Vec4 &a) const {
	return ((x == a.x) && (y == a.y) && (z == a.z) && (w == a.w));
}

inline bool	Vec4::operator==(const Vec4 &rhs) const {
	return Compare(rhs);
}

inline bool	Vec4::operator!=(const Vec4 &rhs) const {
	return !Compare(rhs);
}

inline float Vec4::Dot( const Vec4 &other ) const {
	return (x*other.x + y*other.y + z*other.z + w*other.w);
}
