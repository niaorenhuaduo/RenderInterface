#pragma once

#include "Actor.h"
#include "..\Include\HandleMgr.h"

// TODO: ActorManager::ModifyActor
// TODO: Optimize HandleMgr: Create a big array of DATA/MAGIC data on construction

// interface needed for gamelogic ...
class ActorManager {
public:
	ActorManager();
	virtual ~ActorManager();

	HActor CreateActor(const char* const typeName, tinyxml2::XMLElement *pOverride);
	void   ReleaseActor(const HActor aID);

	void OnUpdate(double timeElapsed);

	// this is not really necessary
	template<typename Component>
	Component*		 GetComponent(HActor id);
	U32				 GetComponentBitfield(HActor id) const;
	const ActorType& GetType(HActor id) const;

	Actor*	   GetActor(HActor id); 
	U32		   GetNumActors() const { return mNumActors; }

	static U32 GetNextComponentID() { return msComponentTypeCounter++; }	// Only used for static component ids
private:
	typedef HandleMgr<Actor, HActor> HActorMgr;

	std::map<U32, U32> mComponentNameIDMapper;					// maps hashed nameid to componentid; only modified in constructor
	const U32 GetComponentID(const char* const pComponentName);	// slow... need another way to do this

	HActorMgr   mActors;
	U32			mNumActors;

	static U32  msComponentTypeCounter;

	void ModifyActor(Actor *pActor, tinyxml2::XMLElement *pOverride);	 // private?
protected:
	GenericObjectFactory<IComponent, U32> mComponentFactory;
	virtual IComponent* VCreateComponent(const tinyxml2::XMLElement *pData);	// Can be overridden to implement game specific components
};

inline Actor* ActorManager::GetActor(HActor id) {
	Actor *pActor = mActors.Get(id);
	assert(pActor && "ActorManager::GetActor: Actor doesn't exist.");
	return pActor;
}

template<typename Component>
inline Component* ActorManager::GetComponent(HActor id) {
	Actor *pActor = mActors.Get(id);
	assert(pActor && "ActorManager::GetComponent: Actor doesn't exist");
	return static_cast<Component*>(pActor->GetComponent(Component::scID));
}

inline U32 ActorManager::GetComponentBitfield(HActor id) const {
	const Actor *pActor = mActors.Get(id);
	assert(pActor && "ActorManager::GetComponentBitfield: Actor doesn't exist");
	return pActor->GetBitField();
}

inline const ActorType& ActorManager::GetType(HActor id) const {
	const Actor *pActor = mActors.Get(id);
	assert(pActor && "ActorManager::GetType: Actor doesn't exist");
	return pActor->GetType();
}