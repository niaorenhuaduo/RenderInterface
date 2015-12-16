#pragma once

#include "..\Include\HandleMgr.h"
#include <vector>

class IComponent;

class Actor  {
	friend class ActorManager;					// for the private destructor ...
	friend class std::allocator<Actor>;			// the user has no right to destroy an actor
	friend class HandleMgr<Actor, HActor>;		// use the actormanager for this
public:
	static const U32 mMaxComponents = 16;

	Actor();
	void OnUpdate(double timeElapsed);

	void AddComponent(IComponent *pComponent);

	HActor			 GetID() const   { return mID; }
	const ActorType& GetType() const { return mType; }

	IComponent* GetComponent(ComponentID id) {
		assert(id >= 0 && id < mMaxComponents);
		assert(mpComponents[id] != NULL && "Actor::GetComponent: Test for bitfield before calling getcomponent.");
		return mpComponents[id];
	}
	U32	GetBitField() const { return mComponentBitfield; }
	bool IsInitialized() const { return !mID.IsNull(); }
private:
	IComponent *mpComponents[mMaxComponents];
	U32			mComponentBitfield;
	HActor		mID;
	ActorType	mType;

private:
	~Actor();
	void	OnRelease();

	HRESULT OnInit(const HActor id, const tinyxml2::XMLElement *pData);
	void    OnPostInit();
};