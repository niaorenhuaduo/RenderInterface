#pragma once

#include "EventManager.h"
#include "..\Memory\StackAllocator.h"

const U32 EVENTMANAGER_EVENTMEMORYSIZE = 1024;	// one kilobyte
const U32 EVENTMANAGER_NUM_QUEUES = 2;

// Sends events to the registered listeners
// The EventManager assumes that the events will get deleted automatically
class EventManager : public IEventManager {
public:
	explicit EventManager(const char *pName, bool bGlobal);
	virtual ~EventManager();

	bool VAddListener(const EventListenerDelegate &delegate, const EventType &type);
	bool VRemoveListener(const EventListenerDelegate &delegate, const EventType &type);

	bool VTriggerEvent(const IEventData* const pEvent) const;
	bool VQueueEvent(const IEventData* const pEvent);		

	bool VOnUpdate();

private:
	typedef std::list<EventListenerDelegate> EventListenerList;
	typedef std::map<EventType, EventListenerList> EventListenerMap;
	typedef std::list<const IEventData*> EventQueue;

	EventListenerMap mEventListeners;
	EventQueue		 mEventQueues[EVENTMANAGER_NUM_QUEUES];
	U32				 mActiveQueue;	// events are put in the opposing queue

	DoubleBufferedAllocator mEventMemory;
	void* AllocMemoryForEvent(U32 size) { return mEventMemory.Alloc(size); }
};
