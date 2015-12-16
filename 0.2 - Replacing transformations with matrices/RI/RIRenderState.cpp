#include "..\Include\RIRenderState.h"


RIRenderState::RIRenderState() : mTexture(), mTransformation(), mbAlpha(true) {}

RIRenderState::RIRenderState( HTexture texture ) : mTexture(texture), mTransformation(), mbAlpha(true) {}

RIRenderState::RIRenderState( const RIMatrix2D &transformation ) : mTexture(), mTransformation(transformation), mbAlpha(true) {}

RIRenderState::RIRenderState( bool bAlpha ) : mTexture(), mTransformation(), mbAlpha(bAlpha) {}

RIRenderState::RIRenderState( const RIMatrix2D &transformation, HTexture texture, bool bAlpha ) : mTexture(texture), mTransformation(transformation), mbAlpha(bAlpha) {}
