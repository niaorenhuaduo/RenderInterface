#pragma once


// TODO: Compare with epsilon

class RIVec4 {
public:
	float x, y, z, w;

	RIVec4() { Zero(); }
	explicit RIVec4(float a) { Set(a, a, a, a); }
	explicit RIVec4(float x, float y, float z, float w) { Set(x, y, z, w); }

	void Set(float x, float y, float z, float w);
	void Zero();

	float Dot(const RIVec4 &other) const;

	// operators
	float	operator[](const U32 index) const;
	float&	operator[](const U32 index);
	RIVec4  operator-() const;	// Negate

	RIVec4  operator+(const RIVec4 &rhs) const;
	RIVec4  operator-(const RIVec4 &rhs) const;
	RIVec4  operator*(const float a) const;
	RIVec4  operator/(const float a) const;
	RIVec4  operator*(const RIVec4 &rhs) const;		
	RIVec4& operator+=(const RIVec4 &rhs);
	RIVec4& operator-=(const RIVec4 &rhs);
	RIVec4& operator*=(const float a);
	RIVec4& operator/=(const float a);
	RIVec4& operator*=(const RIVec4 &rhs);
	RIVec4& operator/=(const RIVec4 &rhs);

	bool	Compare(const RIVec4 &a) const;											// TODO compare with epsilon																		
	bool	operator==(const RIVec4 &rhs) const;	// exact compare!
	bool	operator!=(const RIVec4 &rhs) const;

	// Copy constr/assign
	RIVec4(const RIVec4 &rhs) { x = rhs.x; y = rhs.y; z = rhs.z; w = rhs.w; }
	RIVec4& operator=(const RIVec4 &rhs) { x = rhs.x; y = rhs.y; z = rhs.z; w = rhs.w; return *this;}
};

inline void RIVec4::Set(float x, float y, float z, float w) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

inline void RIVec4::Zero() {
	x = y = z = w = 0.0f;
}

inline float RIVec4::operator[](const U32 index) const {
	return (&x)[index];
}

inline float& RIVec4::operator[](const U32 index) {
	return (&x)[index];
}

inline RIVec4 RIVec4::operator-() const {
	return RIVec4(-x, -y, -z, -w);
}

inline RIVec4 RIVec4::operator+(const RIVec4 &rhs) const {
	return RIVec4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
}

inline RIVec4 RIVec4::operator-(const RIVec4 &rhs) const {
	return RIVec4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
}

inline RIVec4 RIVec4::operator*(const float a) const {
	return RIVec4(x * a, y * a, z * a, w * a);
}

inline RIVec4 RIVec4::operator/(const float a) const {
	float inv = 1.0f/a;
	return RIVec4(x * inv, y * inv, z * inv, w * inv);
}

inline RIVec4 RIVec4::operator*(const RIVec4 &rhs) const {
	return RIVec4(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w);
}

inline RIVec4& RIVec4::operator+=(const RIVec4 &rhs) {
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	w += rhs.w;

	return *this;
}

inline RIVec4& RIVec4::operator-=(const RIVec4 &rhs) {
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	w -= rhs.w;

	return *this;
}

inline RIVec4& RIVec4::operator*=(const float a) {
	x *= a;
	y *= a;
	z *= a;
	w *= a;

	return *this;
}

inline RIVec4& RIVec4::operator/=(const float a) {
	float inva = 1.0f / a;
	x *= inva;
	y *= inva;
	z *= inva;
	w *= inva;

	return *this;
}


inline RIVec4& RIVec4::operator*=( const RIVec4 &rhs )
{
	x *= rhs.x;
	y *= rhs.y;
	z *= rhs.z;
	w *= rhs.w;

	return *this;
}


inline RIVec4& RIVec4::operator/=(const RIVec4 &rhs) {
	x /= rhs.x;
	y /= rhs.y;
	z /= rhs.z;
	w /= rhs.w;

	return *this;
}

inline bool	RIVec4::Compare(const RIVec4 &a) const {
	return ((x == a.x) && (y == a.y) && (z == a.z) && (w == a.w));
}

inline bool	RIVec4::operator==(const RIVec4 &rhs) const {
	return Compare(rhs);
}

inline bool	RIVec4::operator!=(const RIVec4 &rhs) const {
	return !Compare(rhs);
}

inline float RIVec4::Dot( const RIVec4 &other ) const {
	return (x*other.x + y*other.y + z*other.z + w*other.w);
}




/* ----------------------------------- RIVec2 ----------------------------------------- */
class RIVec2 {
public:
	float x, y;

	RIVec2() { Zero(); }
	explicit RIVec2(float a) { Set(a, a); }
	explicit RIVec2(float x, float y) { Set(x, y); }

	void Set(float x, float y);
	void Zero();

	float Dot(const RIVec2 &other) const;

	float ToAngle() { return atan2(y, x); }
	float GetLength() { return sqrt(x*x + y*y); }
	float GetLengthSquared() { return x*x + y*y; }

	// operators
	float	operator[](const U32 index) const;
	float&	operator[](const U32 index);
	RIVec2  operator-() const;	// Negate

	RIVec2  operator+(const RIVec2 &rhs) const;
	RIVec2  operator-(const RIVec2 &rhs) const;
	RIVec2  operator*(const float a) const;
	RIVec2  operator/(const float a) const;
	RIVec2  operator*(const RIVec2 &rhs) const;		
	RIVec2& operator+=(const RIVec2 &rhs);
	RIVec2& operator-=(const RIVec2 &rhs);
	RIVec2& operator*=(const float a);
	RIVec2& operator*=(const RIVec2 &rhs);
	RIVec2& operator/=(const float a);
	RIVec2& operator/=(const RIVec2 &rhs);

	bool	Compare(const RIVec2 &a) const;											// TODO compare with epsilon																		
	bool	operator==(const RIVec2 &rhs) const;	// exact compare!
	bool	operator!=(const RIVec2 &rhs) const;

	// Copy constr/assign
	RIVec2(const RIVec2 &rhs) { x = rhs.x; y = rhs.y; }
	RIVec2& operator=(const RIVec2 &rhs) { x = rhs.x; y = rhs.y; return *this; }
};

inline void RIVec2::Set(float x, float y) {
	this->x = x;
	this->y = y;
}

inline void RIVec2::Zero() {
	x = y = 0.0f;
}

inline float RIVec2::operator[](const U32 index) const {
	return (&x)[index];
}

inline float& RIVec2::operator[](const U32 index) {
	return (&x)[index];
}

inline RIVec2 RIVec2::operator-() const {
	return RIVec2(-x, -y);
}

inline RIVec2 RIVec2::operator+(const RIVec2 &rhs) const {
	return RIVec2(x + rhs.x, y + rhs.y);
}

inline RIVec2 RIVec2::operator-(const RIVec2 &rhs) const {
	return RIVec2(x - rhs.x, y - rhs.y);
}

inline RIVec2 RIVec2::operator*(const float a) const {
	return RIVec2(x * a, y * a);
}

inline RIVec2 RIVec2::operator/(const float a) const {
	float inv = 1.0f/a;
	return RIVec2(x * inv, y * inv);
}

inline RIVec2 RIVec2::operator*(const RIVec2 &rhs) const {
	return RIVec2(x * rhs.x, y * rhs.y);
}

inline RIVec2& RIVec2::operator+=(const RIVec2 &rhs) {
	x += rhs.x;
	y += rhs.y;

	return *this;
}

inline RIVec2& RIVec2::operator-=(const RIVec2 &rhs) {
	x -= rhs.x;
	y -= rhs.y;

	return *this;
}

inline RIVec2& RIVec2::operator*=(const float a) {
	x *= a;
	y *= a;

	return *this;
}

inline RIVec2& RIVec2::operator*=( const RIVec2 &rhs )
{
	x *= rhs.x;
	y *= rhs.y;

	return *this;
}

inline RIVec2& RIVec2::operator/=(const float a) {
	float inva = 1.0f / a;
	x *= inva;
	y *= inva;

	return *this;
}

inline RIVec2& RIVec2::operator/=(const RIVec2 &rhs) {
	x /= rhs.x;
	y /= rhs.y;

	return *this;
}

inline bool	RIVec2::Compare(const RIVec2 &a) const {
	return ((x == a.x) && (y == a.y));
}

inline bool	RIVec2::operator==(const RIVec2 &rhs) const {
	return Compare(rhs);
}

inline bool	RIVec2::operator!=(const RIVec2 &rhs) const {
	return !Compare(rhs);
}

inline float RIVec2::Dot( const RIVec2 &other ) const
{
	return (x*other.x + y*other.y);
}
