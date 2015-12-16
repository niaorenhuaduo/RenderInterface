#pragma once

#include "Fruit.h"


class Level {
public:

	enum Action {
		NOTHING,
		SPAWN_WATERMELON,
		SPAWN_ROCKMELON,
		SPAWN_APPLE,
		SPAWN_BOMB,
		LEVEL_FINISHED
	};

	Level();

	void OnInit(U32 level);

	Action OnUpdate(float fElapsedTime);
	

private:

	struct SpawnObject {
		SpawnObject(float time, Action action) : timeUntilSpawn(time), spawnAction(action) {}

		float       timeUntilSpawn;
		Action		spawnAction;
	};

	std::vector<SpawnObject>    mObjectList;
	float						mElapsedTime;
	U32							mActObject;


};

