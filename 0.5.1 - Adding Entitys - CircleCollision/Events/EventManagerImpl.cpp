#include "..\Global.h"

#include "EventManagerImpl.h"
#include "..\Debugging\Logger.h"

EventManager::EventManager(const char *pName, bool bGlobal) : IEventManager(pName, bGlobal), mActiveQueue(0) {
	FLOG(logINFO) << "Creating EventManager: " << pName;
	assert(EVENTMANAGER_NUM_QUEUES >= 2);

	std::string eventStackName = "DBufferedAllocator for: " + std::string(pName);
	if(FAILED(mEventMemory.OnInit(eventStackName.c_str(), EVENTMANAGER_EVENTMEMORYSIZE))) {
		FLOG(logERROR) << "Couldn't create allocator for events in EventManager: " << pName;
		assert(0 && "EventManager::EventManager: Couldn't create allocator for events");
	}
}
EventManager::~EventManager() {}

bool EventManager::VAddListener(const EventListenerDelegate &delegate, const EventType &type) {
	assert(type && delegate);
	FLOG(logDEBUG) << "Attempting to add delegate-Function for EventType: " << type;

	EventListenerList &eventListenerList = mEventListeners[type];
	for(EventListenerDelegate it : eventListenerList) {
		if(it == delegate) {
			FLOG(logWARNING) << "Trying to double-register a delegate to Event: " << type;
			return false;
		}
	}

	eventListenerList.push_back(delegate);
	FLOG(logDEBUG) << "Successfully added delegate to EventListenerList for type: " << type;
	return true;
}

bool EventManager::VRemoveListener(const EventListenerDelegate &delegate, const EventType &type) {
	assert(type && delegate);
	FLOG(logDEBUG) << "Attempting to remove delegate-Function for EventType: " << type;

	bool success = false;

	auto it = mEventListeners.find(type);
	if(it != mEventListeners.end()) {
		EventListenerList &eventListenerList = it->second;
		for(EventListenerDelegate it : eventListenerList) {
			if(it == delegate) {
				eventListenerList.remove(it);
				success = true;
				FLOG(logDEBUG) << "Successfully removed delegate from EventListenerList for type: " << type;
				break;
			}
		}
	}

	return success;
}



bool EventManager::VTriggerEvent(const IEventData* const pEvent) const {
	assert(pEvent && "EventManager::VTriggerEvent: Null-Pointer Event.");
	FLOG(logDEBUG2) << "Attempting to trigger event: " << pEvent->VGetName();

	bool processed = false;

	auto it = mEventListeners.find(pEvent->VGetType());
	if(it != mEventListeners.end()) {
		const EventListenerList &eventListenerList = it->second;
		for(EventListenerList::const_iterator delegateIt = eventListenerList.begin(); delegateIt != eventListenerList.end(); ++delegateIt) {
			EventListenerDelegate listener = (*delegateIt);
			FLOG(logDEBUG2) << "Sending Event: " << pEvent->VGetName() << " to delegate.";
			listener(pEvent);
			processed = true;
		}
	}

	return processed;
}

bool EventManager::VQueueEvent(const IEventData* const pEvent) {
	assert(mActiveQueue >= 0 && mActiveQueue < EVENTMANAGER_NUM_QUEUES);

	if(!pEvent) {
		FLOG(logWARNING) << "NullPointer Event in VQueueEvent!";
		return false;
	}

	FLOG(logDEBUG2) << "Attempting to queue event: " << pEvent->VGetName();

	auto it = mEventListeners.find(pEvent->VGetType());
	if(it != mEventListeners.end()) {
		mEventQueues[mActiveQueue].push_back(pEvent);
		FLOG(logDEBUG2) << "Successfully queued event: " << pEvent->VGetName();
		return true;
	} else {
		FLOG(logWARNING) << "Ignoring event because there are no listeners for event type: " << pEvent->VGetName();
		return false;
	}
}

bool EventManager::VOnUpdate() {
	U32 queueToProcess = mActiveQueue;
	mActiveQueue = (queueToProcess + 1) % EVENTMANAGER_NUM_QUEUES;
	mEventQueues[mActiveQueue].clear();

	FLOG(logDEBUG) << "EventManager: Processing queued events in queue: " << queueToProcess;

	while(!mEventQueues[queueToProcess].empty()) {
		const IEventData* const pEvent = mEventQueues[queueToProcess].front();
		mEventQueues[queueToProcess].pop_front();

		auto it = mEventListeners.find(pEvent->VGetType());
		if(it != mEventListeners.end()) {
			const EventListenerList &eventListenerList = it->second;
			FLOG(logDEBUG2) << "Sending " << pEvent->VGetName() << "-Event to " << eventListenerList.size() << " listeners.";

			for(EventListenerList::const_iterator delegateIt = eventListenerList.begin(); delegateIt != eventListenerList.end(); ++delegateIt) {
				EventListenerDelegate listener = (*delegateIt);
				listener(pEvent);
			}
		}
	}

	// switch event-buffer
	mEventMemory.SwapBuffers();
	mEventMemory.ClearActBuffer();

	return true;
}