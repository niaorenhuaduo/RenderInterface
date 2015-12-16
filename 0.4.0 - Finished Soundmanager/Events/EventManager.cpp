#include "..\Global.h"

#include "EventManager.h"

static IEventManager *gpEventMgr = NULL;

IEventManager* IEventManager::Get() {
	assert(gpEventMgr && "IEventManager::Get: First create the EventManager");
	return gpEventMgr;
}

IEventManager::IEventManager(const char *pName, bool bGlobal) {
	if(bGlobal) {
		if(gpEventMgr) {
			assert(0 && "Trying to create two global Event Manager!");
			delete gpEventMgr;
		}

		gpEventMgr = this;
	}
}

IEventManager::~IEventManager() {
	if(gpEventMgr == this) gpEventMgr = NULL;
}