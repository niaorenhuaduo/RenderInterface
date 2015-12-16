#pragma once

#include <string>
#include "Level.h"
#include "Constants.h"

#include <Drawable/Sprite.h>
#include <ServiceLocator.h>

#include <Drawable/RectangleShape.h>


#include <memory>

#include "FruitHalf.h"


class Playing {
public:
	Playing() {}

	bool OnInit(int level);
	void OnDestroy();

	void OnUpdate(float fElapsedTime);
	void OnRender();

private:
	ri::HTexture mBgTex;
	ri::HTexture mActorTex;
	ri::HTexture mActorHitTex;
	ri::HTexture mHeartTex;

	ri::Sprite mHeart;
	ri::Sprite mBg;
	ri::Sprite mActor;

	ri::RectangleShape testShape;

	float mTime;

	// fruits
	typedef std::tr1::shared_ptr<Fruit> FruitPtr;
	typedef std::list<FruitPtr> FruitList;

	typedef std::list<FruitHalf> FruitHalfList;


	U32			mLevelNumber;
	Level		mLevel;
	FruitList	mFruits;
	U32			mLifes;
	float		mLifeBarWidth;
	float		mHeartStartX;

	FruitHalfList mHalves;
};