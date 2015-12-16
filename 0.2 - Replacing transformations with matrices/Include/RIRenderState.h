#pragma once

#include "RIRenderInterface.h"

class RIMatrix2D;

class RIRenderState {
public:
	// Default constructor
	// > Default Texture
	// > Identity Transformation
	// > Alphablending is enabled
	RIRenderState();

	// Default Renderstates with one parameter
	RIRenderState(HTexture texture);
	RIRenderState(const RIMatrix2D &transformation);
	RIRenderState(bool bAlpha);

	// Fully custom
	RIRenderState(const RIMatrix2D &transformation, HTexture texture, bool bAlpha);

	HTexture	mTexture;
	RIMatrix2D	mTransformation;
	bool		mbAlpha;
};