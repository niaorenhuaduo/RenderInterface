#include "..\Global.h"

#include "BaseGameLogic.h"
#include "Application.h"
#include "..\Events\Events.h"

using namespace tinyxml2;

BaseGameLogic::BaseGameLogic() : mGameState(BGS_INVALID), mRealTimeSinceGameStarted(0.0), mpActorManager(NULL), mpTimer(NULL) {}
BaseGameLogic::~BaseGameLogic() {
	SAFE_DELETE(mpActorManager);
	if(mpTimer) gpApp->GetClock()->ReleaseTimer(mpTimer);
}

HRESULT BaseGameLogic::Init() {
	if(mGameState != BGS_INVALID) {
		FLOG(logWARNING) << "BaseGameLogic already initialized.";
		return S_OK;
	}
	assert(gpApp);

	mpTimer = gpApp->GetClock()->CreateTimer();
	if(!mpTimer) {
		FLOG(logERROR) << "BaseGameLogic::Init: Clock returned null-Pointer.";
		return E_FAIL;
	}
	mpActorManager	= rnew ActorManager();
	assert(mpActorManager && "BaseGameLogic::Init: Out of memory");
	mGameState		= BGS_INITIALIZING;
	mRealTimeSinceGameStarted = 0.0;

	return S_OK;
}

HRESULT BaseGameLogic::VLoadLevel( const char* const pLevelName )
{
	if(mGameState != BGS_LOADINGGAME || mpActorManager->GetNumActors()) {
		FLOG(logERROR) << "BaseGameLogic::VLoadLevel: GameState isn't BGS_LOADINGGAME or not all actors were released.";
		return E_FAIL;
	}

	std::string filename = "Data\\Levels\\";
	filename += pLevelName;
	filename += ".xml";

	XMLDocument doc;
	XMLError er = doc.LoadFile(filename.c_str());
	if(er != XML_SUCCESS) {
		FLOG(logERROR) << "Failed to load xml File for Level: " << pLevelName;
		FLOG(logERROR) << "ErrorCode: " << er; 
		return E_FAIL;
	}

	XMLElement *pRoot = doc.RootElement();
	if(!pRoot) {
		FLOG(logERROR) << "No root-element for Level: " << pLevelName;
		return E_FAIL;
	}

	// TODO: pre and postload scripts 

	// TODO: Initialize human views

	XMLElement *pStaticActors = pRoot->FirstChildElement("StaticActors");
	if(pStaticActors) {
		for(XMLElement *pActor = pStaticActors->FirstChildElement(); pActor; pActor = pActor->NextSiblingElement()) {
			const char *pActorType = pActor->Attribute("type");

			HActor aID = mpActorManager->CreateActor(pActorType, pActor);
			if(!aID.IsNull()) {
				EvtNewActor *pEvtNewActor = IEventManager::Get()->CreateEvent<EvtNewActor>();
				new(pEvtNewActor) EvtNewActor(aID, INVALID_GAMEVIEW_ID);
				IEventManager::Get()->VQueueEvent(pEvtNewActor);
			}
		}
	}

	if(FAILED(VLoadLevelDelegate(pRoot)))
		return E_FAIL;

	EvtLevelLoaded *pEvent = IEventManager::Get()->CreateEvent<EvtLevelLoaded>();
	new(pEvent) EvtLevelLoaded();
	IEventManager::Get()->VQueueEvent(pEvent);											// Queue or trigger?

	return S_OK;
}

void BaseGameLogic::VOnUpdate()
{
	mRealTimeSinceGameStarted += gpApp->GetClock()->GetRealElapsedTime();		// update the realtime; timer gets updated automatically

	mpActorManager->OnUpdate(mpTimer->GetElapsedTime());
}

void BaseGameLogic::VChangeState( enum BaseGameState state )
{

}
