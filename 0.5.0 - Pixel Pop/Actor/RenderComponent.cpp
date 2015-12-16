#include "..\Global.h"

#include "ActorManager.h"
#include "RenderComponent.h"
#include "..\Debugging\Logger.h"

const U32 RenderComponent2D::scID = ActorManager::GetNextComponentID();
const char *RenderComponent2D::scName = "RenderComponent2D";

using namespace tinyxml2;


RenderComponent2D::RenderComponent2D() : mTextureName(""), mTextureCoordinates(0.0f, 0.0f, 1.0f, 1.0f), mbAlpha(false), mRenderOrder(RIGUIORD_FRONT) {}

HRESULT RenderComponent2D::VOnInit( const tinyxml2::XMLElement *pElement )
{
	const XMLElement *pTexture = pElement->FirstChildElement("Texture");
	if(pTexture) {
		mTextureName = pTexture->FirstChild()->Value();
	}

	const XMLElement *pTexCoords = pElement->FirstChildElement("Texcoords");
	if(pTexCoords) {
		mTextureCoordinates.x = pTexCoords->FloatAttribute("x1");
		mTextureCoordinates.y = pTexCoords->FloatAttribute("y1");
		mTextureCoordinates.z = pTexCoords->FloatAttribute("x2");
		mTextureCoordinates.w = pTexCoords->FloatAttribute("y2");
	}

	const XMLElement *pAlpha = pElement->FirstChildElement("Alpha");
	if(pAlpha) {
		if(strcmp(pAlpha->FirstChild()->Value(), "true") == 0) {
			mbAlpha = true;
		} else mbAlpha = false;
	}

	const XMLElement *pOrder = pElement->FirstChildElement("RenderOrder");
	if(pOrder) {
		U32 order = pOrder->IntAttribute("x");
		if(order < 0 || order >= RIGUIORD_LAST) {
			order = 0;
			FLOG(logWARNING) << "RenderComponent2D: Incompatible renderorder: " << order;
		}
		mRenderOrder = static_cast<guiOrder_t>(order);
	}

	if(!pTexture || !pTexCoords || !pAlpha || !pOrder) {
		FLOG(logWARNING) << "RenderComponent2D::VOnInit: One or more of the data members is corrupt. Owner: " << mpOwner->GetType().c_str();
	}

	return S_OK;
}
