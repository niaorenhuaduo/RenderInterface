inline void Vec2::Set(float x, float y) {
	this->x = x;
	this->y = y;
}

inline void Vec2::Zero() {
	x = y = 0.0f;
}

inline float Vec2::operator[](const U32 index) const {
	return (&x)[index];
}

inline float& Vec2::operator[](const U32 index) {
	return (&x)[index];
}

inline Vec2 Vec2::operator-() const {
	return Vec2(-x, -y);
}

inline Vec2 Vec2::operator+(const Vec2 &rhs) const {
	return Vec2(x + rhs.x, y + rhs.y);
}

inline Vec2 Vec2::operator-(const Vec2 &rhs) const {
	return Vec2(x - rhs.x, y - rhs.y);
}

inline Vec2 Vec2::operator*(const float a) const {
	return Vec2(x * a, y * a);
}

inline Vec2 Vec2::operator/(const float a) const {
	float inv = 1.0f/a;
	return Vec2(x * inv, y * inv);
}

inline Vec2 Vec2::operator*(const Vec2 &rhs) const {
	return Vec2(x * rhs.x, y * rhs.y);
}

inline Vec2& Vec2::operator+=(const Vec2 &rhs) {
	x += rhs.x;
	y += rhs.y;

	return *this;
}

inline Vec2& Vec2::operator-=(const Vec2 &rhs) {
	x -= rhs.x;
	y -= rhs.y;

	return *this;
}

inline Vec2& Vec2::operator*=(const float a) {
	x *= a;
	y *= a;

	return *this;
}

inline Vec2& Vec2::operator*=( const Vec2 &rhs )
{
	x *= rhs.x;
	y *= rhs.y;

	return *this;
}

inline Vec2& Vec2::operator/=(const float a) {
	float inva = 1.0f / a;
	x *= inva;
	y *= inva;

	return *this;
}

inline Vec2& Vec2::operator/=(const Vec2 &rhs) {
	x /= rhs.x;
	y /= rhs.y;

	return *this;
}

inline bool	Vec2::Compare(const Vec2 &a) const {
	return ((x == a.x) && (y == a.y));
}

inline bool	Vec2::operator==(const Vec2 &rhs) const {
	return Compare(rhs);
}

inline bool	Vec2::operator!=(const Vec2 &rhs) const {
	return !Compare(rhs);
}

inline float Vec2::Dot( const Vec2 &other ) const
{
	return (x*other.x + y*other.y);
}