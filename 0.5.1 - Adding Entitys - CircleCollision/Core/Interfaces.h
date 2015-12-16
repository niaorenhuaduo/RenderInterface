#pragma once

class ActorManager;

struct actor_Tag_t {};
typedef Handle<actor_Tag_t> HActor;
typedef std::string			ActorType;

typedef U32 ComponentID;
typedef U32 GameViewID;

const ComponentID INVALID_COMPONENT_ID = 0xffffffff;
const GameViewID  INVALID_GAMEVIEW_ID = 0xffffffff;


class IGameLogic {
public:
	virtual HRESULT VLoadLevel(const char* const pLevelName)=0;

	virtual void	VOnUpdate()=0;
	virtual void	VChangeState(enum BaseGameState state)=0;

	virtual ActorManager* VGetActorManager()=0;		// Interface?
};