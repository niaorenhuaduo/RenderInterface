#pragma once

#include "Component.h"
#include <RIRenderInterface.h>

// TODO: HTexture instead of std::string 

class RenderComponent2D : public IComponent {
public:
	static const U32 scID;
	static const char *scName;

	U32			VGetID() const	 { return scID; }
	const char* VGetName() const { return scName; }

	RenderComponent2D();
	HRESULT VOnInit(const tinyxml2::XMLElement *pElement);

	const std::string& GetTextureName() const { return mTextureName; }
	const RIVec4&	   GetTexCoords() const   { return mTextureCoordinates; }
	const bool		   GetAlpha() const		  { return mbAlpha; }
	const guiOrder_t   GetOrder() const		  { return mRenderOrder; }
private:

	std::string mTextureName;
	RIVec4		mTextureCoordinates;
	bool		mbAlpha;
	guiOrder_t  mRenderOrder;
};