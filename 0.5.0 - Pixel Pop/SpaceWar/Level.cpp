#include "Level.h"

Level::Level() :
	mActObject(0), mElapsedTime(0.0f), mObjectList()	
{}

Level::Action Level::OnUpdate(float fElapsedTime) {
	if(mElapsedTime == -50.0f) mElapsedTime = 0.0f;
	else mElapsedTime += fElapsedTime;

	if(mActObject >= mObjectList.size()) return LEVEL_FINISHED;

	if(mElapsedTime >= mObjectList[mActObject].timeUntilSpawn) {
		mElapsedTime -= mObjectList[mActObject].timeUntilSpawn;
		return mObjectList[mActObject++].spawnAction;
	}

	return NOTHING;
}

void Level::OnInit( U32 level )
{
	assert(level >= 1 && level <= 3);

	mActObject		= 0;
	mElapsedTime	= -50.0f;
	mObjectList.clear();


	assert(SONG1_START - TIME_TO_REACH_HITPOINT > 0);
	assert(SONG2_START - TIME_TO_REACH_HITPOINT > 0);
	assert(POINTLESS_START - TIME_TO_REACH_HITPOINT > 0);

	if(level == 1) {
		mObjectList.push_back(SpawnObject(SONG1_START - TIME_TO_REACH_HITPOINT, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(4.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(4.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(4.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(2.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(2.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(2.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(2.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(2.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(2.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(2.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(2.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(4.0f, SPAWN_APPLE));
		mObjectList.push_back(SpawnObject(4.0f, SPAWN_APPLE));
		mObjectList.push_back(SpawnObject(4.0f, SPAWN_APPLE));
		mObjectList.push_back(SpawnObject(4.0f, SPAWN_APPLE));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_BOMB));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_BOMB));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_BOMB));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_BOMB));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_BOMB));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_BOMB));
		mObjectList.push_back(SpawnObject(2.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(2.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(2.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(2.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(2.0f, SPAWN_WATERMELON));
	} else if(level == 2) {
		mObjectList.push_back(SpawnObject(SONG2_START - TIME_TO_REACH_HITPOINT, SPAWN_WATERMELON));		
		mObjectList.push_back(SpawnObject(4.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(4.0f, SPAWN_ROCKMELON));
		mObjectList.push_back(SpawnObject(4.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(2.0f, SPAWN_BOMB));
		mObjectList.push_back(SpawnObject(2.0f, SPAWN_APPLE));
		mObjectList.push_back(SpawnObject(2.0f, SPAWN_BOMB));
		mObjectList.push_back(SpawnObject(2.0f, SPAWN_APPLE));
		mObjectList.push_back(SpawnObject(2.0f, SPAWN_BOMB));
		mObjectList.push_back(SpawnObject(2.0f, SPAWN_APPLE));
		mObjectList.push_back(SpawnObject(2.0f, SPAWN_BOMB));
		mObjectList.push_back(SpawnObject(2.0f, SPAWN_APPLE));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_ROCKMELON));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_APPLE));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_ROCKMELON));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_APPLE));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_ROCKMELON));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_APPLE));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_ROCKMELON));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_APPLE));
		mObjectList.push_back(SpawnObject(2.0f, SPAWN_APPLE));
		mObjectList.push_back(SpawnObject(2.0f, SPAWN_APPLE));
		mObjectList.push_back(SpawnObject(2.0f, SPAWN_APPLE));
		mObjectList.push_back(SpawnObject(2.0f, SPAWN_APPLE));
		mObjectList.push_back(SpawnObject(2.0f, SPAWN_APPLE));
		mObjectList.push_back(SpawnObject(2.0f, SPAWN_APPLE));
		mObjectList.push_back(SpawnObject(2.0f, SPAWN_APPLE));
		mObjectList.push_back(SpawnObject(2.0f, SPAWN_APPLE));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_BOMB));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_BOMB));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_APPLE));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_BOMB));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_BOMB));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_BOMB));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_APPLE));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_APPLE));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_APPLE));
		mObjectList.push_back(SpawnObject(1.0f, SPAWN_BOMB));
		mObjectList.push_back(SpawnObject(2.0f, SPAWN_ROCKMELON));
		mObjectList.push_back(SpawnObject(2.0f, SPAWN_ROCKMELON));
		mObjectList.push_back(SpawnObject(2.0f, SPAWN_ROCKMELON));
		mObjectList.push_back(SpawnObject(2.0f, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(2.0f, SPAWN_APPLE));
	} else if(level == 3) {

/*
		mObjectList.push_back(SpawnObject(POINTLESS_START - TIME_TO_REACH_HITPOINT, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_4, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_4, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_4, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_4, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_4, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_4, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_4, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_4, SPAWN_WATERMELON));*/

		mObjectList.push_back(SpawnObject(POINTLESS_START - TIME_TO_REACH_HITPOINT, SPAWN_WATERMELON));

		mObjectList.push_back(SpawnObject(POINTLESS_2, SPAWN_APPLE));
		mObjectList.push_back(SpawnObject(POINTLESS_2, SPAWN_APPLE));
		mObjectList.push_back(SpawnObject(POINTLESS_2, SPAWN_APPLE));
		mObjectList.push_back(SpawnObject(POINTLESS_2, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_2, SPAWN_ROCKMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_2, SPAWN_ROCKMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_2, SPAWN_ROCKMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_2, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_2, SPAWN_APPLE));
		mObjectList.push_back(SpawnObject(POINTLESS_2, SPAWN_APPLE));
		mObjectList.push_back(SpawnObject(POINTLESS_2, SPAWN_APPLE));
		mObjectList.push_back(SpawnObject(POINTLESS_2, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_2, SPAWN_ROCKMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_2, SPAWN_ROCKMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_2, SPAWN_ROCKMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_2, SPAWN_WATERMELON));


		mObjectList.push_back(SpawnObject(3.075f + POINTLESS_2, SPAWN_WATERMELON));		// was 3.152

/*
		mObjectList.push_back(SpawnObject(POINTLESS_2, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_2, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_2, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_2, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_2, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_2, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_2, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_2, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_2, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_2, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_2, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_2, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_2, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_2, SPAWN_WATERMELON));*/

		mObjectList.push_back(SpawnObject(POINTLESS_1, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_1, SPAWN_APPLE));
		mObjectList.push_back(SpawnObject(POINTLESS_1, SPAWN_ROCKMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_1, SPAWN_BOMB));
		mObjectList.push_back(SpawnObject(POINTLESS_1, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_1, SPAWN_APPLE));
		mObjectList.push_back(SpawnObject(POINTLESS_1, SPAWN_ROCKMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_1, SPAWN_BOMB));
		mObjectList.push_back(SpawnObject(POINTLESS_1, SPAWN_APPLE));
		mObjectList.push_back(SpawnObject(POINTLESS_1, SPAWN_APPLE));
		mObjectList.push_back(SpawnObject(POINTLESS_1, SPAWN_ROCKMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_1, SPAWN_ROCKMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_1, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_1, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_1, SPAWN_BOMB));
		mObjectList.push_back(SpawnObject(POINTLESS_1, SPAWN_BOMB));
		mObjectList.push_back(SpawnObject(POINTLESS_1, SPAWN_ROCKMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_1, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_1, SPAWN_ROCKMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_1, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_1, SPAWN_BOMB));
		mObjectList.push_back(SpawnObject(POINTLESS_1, SPAWN_APPLE));
		mObjectList.push_back(SpawnObject(POINTLESS_1, SPAWN_APPLE));
		mObjectList.push_back(SpawnObject(POINTLESS_1, SPAWN_ROCKMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_1, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_1, SPAWN_ROCKMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_1, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_1, SPAWN_ROCKMELON));


		mObjectList.push_back(SpawnObject(POINTLESS_4, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_4, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_4, SPAWN_ROCKMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_4, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_4, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_4, SPAWN_ROCKMELON));		// 54.7665

		mObjectList.push_back(SpawnObject(POINTLESS_1, SPAWN_ROCKMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_1, SPAWN_APPLE));
		mObjectList.push_back(SpawnObject(POINTLESS_1, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_2, SPAWN_ROCKMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_1, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_1, SPAWN_APPLE));
		mObjectList.push_back(SpawnObject(POINTLESS_1, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_2, SPAWN_ROCKMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_1, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_1, SPAWN_APPLE));
		mObjectList.push_back(SpawnObject(POINTLESS_1, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_2, SPAWN_ROCKMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_1, SPAWN_ROCKMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_1, SPAWN_APPLE));
		mObjectList.push_back(SpawnObject(POINTLESS_1, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_2, SPAWN_ROCKMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_1, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_1, SPAWN_APPLE));
		mObjectList.push_back(SpawnObject(POINTLESS_1, SPAWN_ROCKMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_2, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_1, SPAWN_ROCKMELON));


		mObjectList.push_back(SpawnObject(POINTLESS_4, SPAWN_ROCKMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_1, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_3, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_1, SPAWN_ROCKMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_3, SPAWN_ROCKMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_1, SPAWN_APPLE));
		mObjectList.push_back(SpawnObject(POINTLESS_3, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_1, SPAWN_ROCKMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_3, SPAWN_ROCKMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_1, SPAWN_APPLE));
		mObjectList.push_back(SpawnObject(POINTLESS_3, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_1, SPAWN_ROCKMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_3, SPAWN_WATERMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_1, SPAWN_BOMB));
		mObjectList.push_back(SpawnObject(POINTLESS_3, SPAWN_ROCKMELON));

/*
		mObjectList.push_back(SpawnObject(POINTLESS_4, SPAWN_ROCKMELON));	// 1m 6.34s
		mObjectList.push_back(SpawnObject(POINTLESS_4, SPAWN_ROCKMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_4, SPAWN_ROCKMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_4, SPAWN_ROCKMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_4, SPAWN_ROCKMELON));	
		mObjectList.push_back(SpawnObject(POINTLESS_4, SPAWN_ROCKMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_4, SPAWN_ROCKMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_4, SPAWN_ROCKMELON));*/


		mObjectList.push_back(SpawnObject(2.228f, SPAWN_WATERMELON));		// 1m 22.812s
		mObjectList.push_back(SpawnObject(POINTLESS_4, SPAWN_ROCKMELON));
		mObjectList.push_back(SpawnObject(POINTLESS_4, SPAWN_APPLE));

		mObjectList.push_back(SpawnObject(3.589f, SPAWN_WATERMELON));		// 1m 29.94s


		mObjectList.push_back(SpawnObject(3.589f, SPAWN_WATERMELON));



	}

}
