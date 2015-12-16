inline Vertex2D::Vertex2D() :
mPos(), 
mTex(), 
mColor(Color::White) {}

inline Vertex2D::Vertex2D( const Vec2 &pos ) : 
mPos(pos),
mTex(), 
mColor(Color::White) {}

inline Vertex2D::Vertex2D( const Vec2 &pos, const Vec2 &tex ) : 
mPos(pos), 
mTex(tex), 
mColor(Color::White) {}

inline Vertex2D::Vertex2D( const Vec2 &pos, const Color &color ) : 
mPos(pos), 
mTex(), 
mColor(color) {}

inline Vertex2D::Vertex2D( const Vec2 &pos, const Vec2 &tex, const Color &color ) : 
mPos(pos), 
mTex(tex), 
mColor(color) {}
