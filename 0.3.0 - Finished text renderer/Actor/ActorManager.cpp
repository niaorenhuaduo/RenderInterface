
#include "..\Global.h"

#include "Component.h"
#include "ActorManager.h"
#include "..\Debugging\Logger.h"
#include "..\Utilities\String.h"

U32 ActorManager::msComponentTypeCounter = 0;

#include "RenderComponent.h"


ActorManager::ActorManager() : mNumActors(0) {
	// Register all components
	mComponentFactory.Register<RenderComponent2D>(HashedString(RenderComponent2D::scName).getHashValue());
	mComponentNameIDMapper[HashedString(RenderComponent2D::scName).getHashValue()] = RenderComponent2D::scID;
}

ActorManager::~ActorManager() {
	if(mActors.HasUsedHandles()) {
		FLOG(logWARNING) << "ActorManager: Not all actors were released!";
	}
}


HActor ActorManager::CreateActor(const char* const typeName, tinyxml2::XMLElement *pOverride) {
	std::string filename = "Data\\Actors\\";
	filename += typeName;
	filename += ".xml";

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError er = doc.LoadFile(filename.c_str());
	if(er != tinyxml2::XML_SUCCESS) {
		FLOG(logERROR) << "Failed to load xml File for ActorType: " << typeName;
		FLOG(logERROR) << "ErrorCode: " << er; 
		return HActor();
	}

	tinyxml2::XMLElement *pRoot = doc.RootElement();
	if(!pRoot) {
		FLOG(logERROR) << "No root-element for ActorType: " << typeName;
		return HActor();
	}

	HActor handle;
	Actor *pActor = mActors.Acquire(handle);
	assert(pActor && "ActorManager::CreateActor: mActors.Acquire returns nullpointer.");
	if(FAILED(pActor->OnInit(handle, pRoot))) {
		FLOG(logERROR) << "Failed to initialize actor: " << typeName;
		mActors.Release(handle);
		return HActor();
	}

	// add components from file
	for(tinyxml2::XMLElement *pNode = pRoot->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement()) {
		assert(pNode);
		IComponent *pComponent = VCreateComponent(pNode);
		if(pComponent) {
			pComponent->SetOwner(pActor);		// Die reihenfolge darf nicht geändert werden
			pActor->AddComponent(pComponent);
		} else {
			FLOG(logERROR) << "ActorManager::CreateActor: One of the Components failed to initialize";
			mActors.Release(handle);
			return HActor();
		}
	}

	if(pOverride) {
		ModifyActor(pActor, pOverride);
	}

	// now run postInit for the components
	pActor->OnPostInit();

	mNumActors++;

	return handle;
}

void ActorManager::ReleaseActor(const HActor aID) {
	Actor *pActor = mActors.Get(aID);
	if(!pActor) {
		FLOG(logWARNING) << "Attempting to release an Actor that doesn't exist. ActorID: " << aID;
		return;
	}

	pActor->OnRelease();
	mActors.Release(aID);
	mNumActors--;
}

IComponent* ActorManager::VCreateComponent(const tinyxml2::XMLElement *pData) {
	const char* pName = pData->Value();	// Name of the component
	IComponent *pComponent = mComponentFactory.Create(HashedString(pName).getHashValue());

	if(!pComponent) {
		FLOG(logERROR) << "Component with name " << pName << " not registered.";
		return NULL;
	}

	if(FAILED(pComponent->VOnInit(pData))) {
		FLOG(logERROR) << "Component failed to initialize: " << pName;
		SAFE_DELETE(pComponent);
		return NULL;
	}

	return pComponent;
}

void ActorManager::ModifyActor( Actor *pActor, tinyxml2::XMLElement *pOverride )
{
	// pOverrides is the actor element; loop through all child==component elements
	for(tinyxml2::XMLElement *pNodeComponent = pOverride->FirstChildElement(); pNodeComponent; pNodeComponent = pNodeComponent->NextSiblingElement()) {
		const char *pName = pNodeComponent->Value();
		U32 componentID = GetComponentID(pName);
		if(componentID == INVALID_COMPONENT_ID) continue;

		IComponent *pComponent = NULL;
		U32 componentBit = IComponent::GetBit(componentID);
		if((pActor->GetBitField() & componentBit) != 0) {
			// Component already exist
			pComponent = pActor->GetComponent(componentID);
			if(FAILED(pComponent->VOnInit(pNodeComponent))) {
				FLOG(logWARNING) << "ModifyActor: One of the components couldn't get initialized correctly.";
			}
			pComponent->VOnChanged();
		} else {
			pComponent = VCreateComponent(pNodeComponent);
			if(pComponent) {
				pActor->AddComponent(pComponent);
				pComponent->SetOwner(pActor);
			}
		}
	}
}

void ActorManager::OnUpdate(double timeElapsed) {
	HActorMgr::iterator it = mActors.begin();
	HActorMgr::iterator end = mActors.end();
	while(it != end) {
		it->OnUpdate(timeElapsed);
		++it;
	}
}

const U32 ActorManager::GetComponentID( const char* const pComponentName )
{
	U32 nameID = HashedString(pComponentName).getHashValue();
	auto it = mComponentNameIDMapper.find(nameID);
	if(it == mComponentNameIDMapper.end()) {
		FLOG(logERROR) << "GetComponentID: No registered component with this name; name: " << pComponentName;
		return INVALID_COMPONENT_ID;
	}

	return it->second;
}
