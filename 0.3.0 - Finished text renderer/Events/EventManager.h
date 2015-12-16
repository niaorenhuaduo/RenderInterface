#pragma once

#include <strstream>
#include "..\3rdParty\fastdelegate\FastDelegate.h"
#include "..\Global.h"

class IEventData; 

typedef U64 EventType;
typedef fastdelegate::FastDelegate1<const IEventData*> EventListenerDelegate;
typedef std::queue<IEventData*> EventTypeQueue;	// Not thread-safe

// Remember: 
class IEventData {
public:
	virtual void VSerialize(std::ostrstream &os) const=0;
	virtual void VDeserialize(std::istrstream &in)=0;

	virtual const EventType& VGetType() const=0;
	virtual const char* VGetName() const=0;

	virtual IEventData* VCopy() const=0;

private:
	virtual ~IEventData() { assert(0 && "Never call the Destructor of an Event!"); }
};



// EventManager Interface
class IEventManager {
public:
	explicit IEventManager(const char *pName, bool bGlobal);
	virtual ~IEventManager();

	virtual bool VAddListener(const EventListenerDelegate &delegate, const EventType &type)=0;		// true if successful
	virtual bool VRemoveListener(const EventListenerDelegate &delegate, const EventType &type)=0;	// false if the pairing was not found

	virtual bool VTriggerEvent(const IEventData* const pData) const=0;
	virtual bool VQueueEvent(const IEventData* const pData)=0;			

	virtual bool VOnUpdate()=0;	// Process queued events

	static IEventManager* Get();

	template<typename T>	// T = class of the event
	T* CreateEvent();		// creates an temporary event for 2 frames; don't delete it; Call the constructor yourself

protected:
	virtual void* AllocMemoryForEvent(U32 size)=0;
};

template<typename T>
inline T* IEventManager::CreateEvent() {
	T* pEvent = static_cast<T*>(AllocMemoryForEvent(sizeof(T)));
	return pEvent;
}