inline RenderState::RenderState() : 
mTexture		(), 
mTransformation	(), 
mbAlpha			(true) {}

inline RenderState::RenderState( HTexture texture ) : 
mTexture		(texture), 
mTransformation	(), 
mbAlpha			(true) {}

inline RenderState::RenderState( const Matrix2D &transformation ) : 
mTexture		(), 
mTransformation	(transformation), 
mbAlpha			(true) {}

inline RenderState::RenderState( bool bAlpha ) : 
mTexture		(), 
mTransformation	(), 
mbAlpha			(bAlpha) {}

inline RenderState::RenderState( const Matrix2D &transformation, HTexture texture, bool bAlpha) : 
mTexture		(texture), 
mTransformation	(transformation), 
mbAlpha			(bAlpha) {}