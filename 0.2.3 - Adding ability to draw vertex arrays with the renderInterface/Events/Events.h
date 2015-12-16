#pragma once

#include "EventManager.h"
#include "..\Core\Interfaces.h"


// TODO: VCopy

class EvtNewActor : public IEventData {
public:
	static const EventType scEventType;
	const EventType& VGetType() const { return scEventType; }
	const char*		 VGetName() const { return "EvtNewActor"; }

	// EvtNewActor() : mActorID(), mGameViewID(INVALID_GAMEVIEW_ID) { }
	explicit EvtNewActor(const HActor aID, const GameViewID gvID) : mActorID(aID), mGameViewID(gvID) {}

	void VSerialize(std::ostrstream &os) const {
		os << mActorID << " ";
		os << mGameViewID << " ";
	}

	void VDeserialize(std::istrstream &in) {
		U32 actorHanlde;
		in >> actorHanlde;
		mActorID.Reset(actorHanlde);
		in >> mGameViewID;
	}

	IEventData* VCopy() const { assert(0 && "TODO: EvtNewActor::VCopy!!"); return NULL;}

private:
	HActor	   mActorID;
	GameViewID mGameViewID;
};

class EvtActorDestroyed : public IEventData {
public:
	static const EventType scEventType;
	const EventType& VGetType() const { return scEventType; }
	const char*		 VGetName() const { return "EvtActorDestroyed"; }

	explicit EvtActorDestroyed(const HActor aID) : mActorID(aID) {}

	void VSerialize(std::ostrstream &os) const {
		os << mActorID << " ";
	}

	void VDeserialize(std::istrstream &in) {
		U32 actorHandle;
		in >> actorHandle;
		mActorID.Reset(actorHandle);
	}

	IEventData* VCopy() const { assert(0 && "TODO: EvtActorDestroyed::VCopy!!"); return NULL;}

private:
	HActor mActorID;
};

class EvtLevelLoaded : public IEventData {
public:
	static const EventType scEventType;
	const EventType& VGetType() const { return scEventType; }
	const char*		 VGetName() const { return "EvtLevelLoaded"; }

	void VSerialize(std::ostrstream &os) const {}
	void VDeserialize(std::istrstream &in) {}

	EvtLevelLoaded() {}
	IEventData* VCopy() const { 
		IEventData *pData = IEventManager::Get()->CreateEvent<EvtLevelLoaded>();
		new(pData) EvtLevelLoaded();
		return pData;
	}
};

class EvtPause : public IEventData {
public:
	static const EventType scEventType;
	const EventType &VGetType() const { return scEventType; }
	const char*		 VGetName() const { return "EvtPause"; }

	EvtPause() : mbPaused(false) {}
	explicit EvtPause(bool bPause) : mbPaused(bPause) {}

	void VSerialize(std::ostrstream &os) const {
		os << mbPaused << " ";
	}

	void VDeserialize(std::istrstream &in) {
		in >> mbPaused;
	}

	IEventData* VCopy() const { assert(0 && "TODO: EvtNewActor::VCopy!!"); return NULL; }
private:
	bool mbPaused;
};