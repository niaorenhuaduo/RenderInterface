#include "Playing.h"
#include "Spaceship.h"

bool Playing::OnInit( int level )
{
	// textures
	mBgTex = ri::ServiceLocator::TexMgr().GetTexture("bg.png");
	mActorTex = ri::ServiceLocator::TexMgr().GetTexture("Actor.png");
	mActorHitTex = ri::ServiceLocator::TexMgr().GetTexture("ActorHit.png");
	mHeartTex = ri::ServiceLocator::TexMgr().GetTexture("Heart.png");

	// sprites
	mBg.SetTexture(mBgTex);
	mBg.SetPosition(BACKGROUND_POS);

	mActor.SetTexture(mActorTex);
	mActor.SetAlpha(true);
	mActor.SetPosition(BODY_POS);


	mHeart.SetTexture(mHeartTex);
	mHeart.SetAlpha(true);
	ri::FloatRect heartRect = mHeart.GetLocalBoundingBox();
	mLifeBarWidth = MAX_HEALTH * heartRect.width + (MAX_HEALTH-1) * 3.0f;
	mHeartStartX = ri::VIRTUALXRESOLUTION / 2.0f - mLifeBarWidth / 2.0f;


	testShape.SetSize(1.0f, 1080.0f);
	testShape.SetPosition(HITPOINT_X, 0.0f);

	mLifes = MAX_HEALTH;
	mTime  = -50.0f;


	mLevel.OnInit(level);
	mLevelNumber = level;

	return true;
}

void Playing::OnDestroy()
{
	gpPixelPop->GetAudio()->StopCue(BACKGROUND_1AND2);
	gpPixelPop->GetAudio()->StopCue(BACKGROUND_3);

	ri::ServiceLocator::TexMgr().ReleaseTexture(mBgTex);
	ri::ServiceLocator::TexMgr().ReleaseTexture(mActorTex);
	ri::ServiceLocator::TexMgr().ReleaseTexture(mActorHitTex);
	ri::ServiceLocator::TexMgr().ReleaseTexture(mHeartTex);


	mFruits.clear();
	mHalves.clear();
}

void Playing::OnUpdate( float fElapsedTime )
{
	if(gpPixelPop->GetInput()->IsKeyDown(VK_ESCAPE)) { gpPixelPop->VSetGameState(SpaceShip::GS_MENU); return; }

	if(mTime == -50.0f) {
		if(mLevelNumber <= 2) gpPixelPop->GetAudio()->PlayCue(BACKGROUND_1AND2);
		else gpPixelPop->GetAudio()->PlayCue(BACKGROUND_3);
		mTime = 0.0f;
	} else 	mTime += fElapsedTime;


	static float animationTimer = 0.0f;
	static bool  bHitAnimation  = false;
	static bool  bPressedLastFrame = false;

	if(bHitAnimation) {
		animationTimer += fElapsedTime;

		if(animationTimer >= ANIMATION_HIT_DURATION) {
			animationTimer = 0.0f;
			bHitAnimation  = false;

			mActor.SetTexture(mActorTex);
		}
	}

	bool bPressedKey = (gpPixelPop->GetInput()->IsKeyDown(VK_SPACE) || gpPixelPop->GetInput()->IsKeyDown(VK_RIGHT) || gpPixelPop->GetInput()->IsKeyDown(VK_UP));

	if(bPressedKey && !bPressedLastFrame && !bHitAnimation) {
		mActor.SetTexture(mActorHitTex);
		animationTimer = 0.0f;
		bHitAnimation = true;

		for(auto it = mFruits.begin(); it != mFruits.end();) {
			float minX = (*it)->GetPosition().x - (*it)->GetHitboxWidth() / 2.0f;
			float maxX = (*it)->GetPosition().x + (*it)->GetHitboxWidth() / 2.0f;

			if(minX < HITPOINT_X && maxX > HITPOINT_X) {
				// we hit something

				if((*it)->GetType() == Fruit::WATERMELON) {

					if(gpPixelPop->GetInput()->IsKeyDown(VK_SPACE)) {
						gpPixelPop->GetAudio()->PlayCue(HIT);

						mHalves.push_back(FruitHalf(Fruit::WATERMELON, true, (*it)->GetPosition()));
						mHalves.push_back(FruitHalf(Fruit::WATERMELON, false, (*it)->GetPosition()));

					} else {
						gpPixelPop->GetAudio()->PlayCue(EXPLOSION);
						mLifes--;
					}

					it = mFruits.erase(it);
					break;
				}


				if((*it)->GetType() == Fruit::APPLE) {

					if(gpPixelPop->GetInput()->IsKeyDown(VK_RIGHT)) {
						gpPixelPop->GetAudio()->PlayCue(HIT);

						mHalves.push_back(FruitHalf(Fruit::APPLE, true, (*it)->GetPosition()));
						mHalves.push_back(FruitHalf(Fruit::APPLE, false, (*it)->GetPosition()));

					} else {
						gpPixelPop->GetAudio()->PlayCue(EXPLOSION);
						mLifes--;
					}

					it = mFruits.erase(it);
					break;
				}

				if((*it)->GetType() == Fruit::ROCKMELON) {

					if(gpPixelPop->GetInput()->IsKeyDown(VK_UP)) {
						gpPixelPop->GetAudio()->PlayCue(HIT);

						mHalves.push_back(FruitHalf(Fruit::ROCKMELON, true, (*it)->GetPosition()));
						mHalves.push_back(FruitHalf(Fruit::ROCKMELON, false, (*it)->GetPosition()));

					} else {
						gpPixelPop->GetAudio()->PlayCue(EXPLOSION);
						mLifes--;
					}

					it = mFruits.erase(it);
					break;
				}


				// everything else is a bomb
				gpPixelPop->GetAudio()->PlayCue(EXPLOSION);
				mLifes--;
	
				it = mFruits.erase(it);
				break;
			} else ++it;
		}
	}
	bPressedLastFrame = bPressedKey;



	// add new fruits when necessary
	Level::Action action = mLevel.OnUpdate(fElapsedTime);
	switch(action) {
	case Level::SPAWN_WATERMELON:	mFruits.push_back(FruitPtr(rnew Watermelon));		break;
	case Level::SPAWN_ROCKMELON:			mFruits.push_back(FruitPtr(rnew Rockmelon));		break;
	case Level::SPAWN_APPLE:		mFruits.push_back(FruitPtr(rnew Apple));			break;
	case Level::SPAWN_BOMB:			mFruits.push_back(FruitPtr(rnew Bomb));				break;
	}


	// update all fruits
	for(auto it = mFruits.begin(); it != mFruits.end(); ++it) {
		(*it)->OnUpdate(fElapsedTime);
	}
	
	// update all halves
	for(auto it = mHalves.begin(); it != mHalves.end();) {
		if(it->IsExpired()) 
			it = mHalves.erase(it);
		else {
			it->OnUpdate(fElapsedTime);
			++it;
		}
	}

	// remove missed fruits
	if(!mFruits.empty()) {
		FruitPtr pFruit = mFruits.front();

		if(pFruit->GetPosition().x < DEADZONE_X) {
			mFruits.pop_front();

			if(pFruit->GetType() != Fruit::BOMB) mLifes--;
		} 
	}


	if(mLifes == 0) {
		Sleep(50);
		gpPixelPop->VSetGameState(SpaceShip::GS_ENDSCREEN_LOST);
		return;
	}

	if(action == Level::LEVEL_FINISHED && mFruits.empty()) {
		Sleep(50);
		gpPixelPop->VSetGameState(SpaceShip::GS_ENDSCREEN_WON);
		return;
	}
}

void Playing::OnRender()
{
	mBg.OnDraw();
	mActor.OnDraw();
	mHeart.OnDraw();
	//testShape.OnDraw();


	// draw all fruits
	for(auto it = mFruits.begin(); it != mFruits.end(); ++it) {
		(*it)->OnDraw();
	}

	// draw all halves
	for(auto it = mHalves.begin(); it != mHalves.end(); ++it) {
		it->OnDraw();
	}

	// draw the hearts
	float heartWidth = mHeart.GetLocalBoundingBox().width;

	float actPosX = mHeartStartX;
	for(U32 i=0; i<mLifes; ++i) {
		mHeart.SetPosition(actPosX, HEART_START_Y);
		mHeart.OnDraw();

		actPosX += heartWidth;
	}
}


