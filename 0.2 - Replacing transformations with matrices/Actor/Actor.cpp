#include "..\Global.h"

#include "Actor.h"

#include "Component.h"
#include "..\Include\HandleMgr.h"
#include "..\Debugging\Logger.h"

Actor::Actor() : mID(), mComponentBitfield(0), mType("Unknown") { 
	for(int i=0; i<mMaxComponents; ++i) {
		mpComponents[i] = NULL;
	}
}
Actor::~Actor() {
	// don't release memory because this class is used in a vector
	// leaks memory when called before OnRelease
}

void Actor::OnRelease() {
	FLOG(logDEBUG2) << "Destroying Actor of type: " << mType.c_str() << "; ID: " << mID;

	for(int i=0; i<mMaxComponents; ++i) {
		if(mpComponents[i]) {
			delete mpComponents[i];
			mpComponents[i] = NULL;
		}
	}

	mComponentBitfield = 0;
	mID.Reset(0);
	mType = "Unknown";
}

HRESULT Actor::OnInit(const HActor id, const tinyxml2::XMLElement *pData) {
	if(mType != "Unknown") {
		FLOG(logERROR) << "Trying to initialize an Actor that is already initialized. ActorType: " << mType.c_str();
		return E_FAIL;
	}
	mID.Reset(id);
	mType = pData->Attribute("type");
	mComponentBitfield = 0;

	FLOG(logDEBUG2) << "Initializing Actor with ID: " << mID << "; Type: " << mType.c_str();
	
	// The factory creates the components
	return S_OK;
}

void Actor::OnPostInit() {
	for(int i=0; i<mMaxComponents; ++i) {
		if(mpComponents[i]) 
			mpComponents[i]->VOnPostInit();
	}

	FLOG(logDEBUG2) << "Successfully created actor with ID: " << mID << "; Type: " << mType.c_str();
}

void Actor::OnUpdate(double timeElapsed) {
	for(int i=0; i<mMaxComponents; ++i) {
		if(mpComponents[i]) 
			mpComponents[i]->VOnUpdate(timeElapsed);
	}
}

void Actor::AddComponent(IComponent *pComponent) {
	assert(pComponent && "Actor::AddComponent: NullPointer");
	U32 componentBitField = pComponent->GetBit(pComponent->VGetID());
	if((componentBitField & mComponentBitfield) != 0) {
		FLOG(logWARNING) << "Trying to add two components of the same type to one actor... ActorType: " << mType.c_str() << "; ComponentType: " << pComponent->VGetName();
		SAFE_DELETE(pComponent);
		return;
	}

	FLOG(logDEBUG2) << "Adding component of type " << pComponent->VGetName() << " to actor with ID: " << mID << " and type " << mType.c_str();
	mpComponents[pComponent->VGetID()] = pComponent;
	mComponentBitfield |= pComponent->GetBit(pComponent->VGetID());
}