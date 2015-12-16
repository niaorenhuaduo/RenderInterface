#pragma once

class Actor;
class tinyxml2::XMLElement;


// Every derived class has to implement:
// static const U32 scID		- component id,  initialize it with ActorManager::GetNextComponentID()
// static const char *spName	- classname
// VOnInit has to work when called multiple times
class IComponent {
	friend class Actor;
	friend class ActorManager;
public:
	virtual HRESULT VOnInit(const tinyxml2::XMLElement *pElement)=0;		
	virtual void    VOnPostInit() {}
	virtual void	VOnUpdate(double timeElapsed) {}
	virtual void	VOnChanged() {}

	virtual ComponentID VGetID() const=0;		// Used as index into actorcomponents
	virtual const char* VGetName() const=0;

	static  U32			GetBit(const ComponentID id) { assert(id < 32 && "IComponent::GetBit"); return (0x1 << id);}	// Used for actor bitfields
protected:
	Actor *mpOwner;
	virtual ~IComponent() {}

private:		// only for the actor class
	void SetOwner(Actor* pActor) { assert(pActor); mpOwner = pActor; }
};