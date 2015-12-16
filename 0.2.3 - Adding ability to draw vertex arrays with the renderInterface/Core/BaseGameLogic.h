#pragma once

// TODO: add script support
// TODO: VLoadLevel: EvtLevelLoaded trigger or queue?

#include "..\Actor\ActorManager.h"
#include "..\Events\EventManager.h"
#include "..\Utilities\Clock.h"

enum BaseGameState
{
	BGS_INVALID,
	BGS_INITIALIZING,
	BGS_MAINMENU,
	BGS_LOADINGGAME,
	BGS_SPAWNINGPLAYERSACTORS,
	BGS_RUNNING
};

class BaseGameLogic : public IGameLogic {
public:
	BaseGameLogic();
	virtual ~BaseGameLogic();

	HRESULT Init();
	HRESULT VLoadLevel(const char* const pLevelName);

	void VOnUpdate();

	virtual ActorManager* VGetActorManager() { assert(mpActorManager); return mpActorManager; }

protected:
	double		  mRealTimeSinceGameStarted;		// real time
	Timer		 *mpTimer;							// GameTimer; can be scaled
	ActorManager *mpActorManager;

	BaseGameState mGameState;

	virtual HRESULT VLoadLevelDelegate(tinyxml2::XMLElement *pLevelData) { return S_OK; }	// Gamespecific loading
	virtual void	VChangeState(enum BaseGameState state);
};