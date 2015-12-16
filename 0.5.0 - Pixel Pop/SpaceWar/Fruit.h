#pragma once

#include <Drawable/Sprite.h>
#include <ServiceLocator.h>
#include "Constants.h"


class Fruit : public ri::Sprite {
public:
	Fruit() : ri::Sprite(), counter(0.0f) {}

	enum type {
		WATERMELON,
		ROCKMELON,
		APPLE,
		BOMB,
		SPAWN_NONE
	};

	virtual void  OnUpdate(float fElapsedTime) {
		ri::Sprite::OnUpdate(fElapsedTime);

		counter += fElapsedTime * 2.0f;
		if(counter > 2.0f * ri::PI) counter -= 2.0f * ri::PI;

		SetPosition(GetPosition().x - MOVEMENT_SPEED * fElapsedTime, BASELINE + cosf(counter) * 150.0f);
	}

	virtual type  GetType() const=0;
	virtual float GetHitboxWidth() const=0;

private:
	float counter;
};


class Watermelon : public Fruit {
public:
	Watermelon() : Fruit() {
		if(tex.IsNull()) tex = ri::ServiceLocator::TexMgr().GetTexture("Watermelon.png");

		SetTexture(tex);
		SetAlpha(true);
		SetOrigin(GetLocalBoundingBox().width / 2.0f, GetLocalBoundingBox().height / 2.0f);
		SetPosition(SPAWN_POS);
	}


	virtual type  GetType() const { return WATERMELON; }
	virtual float GetHitboxWidth() const { return 90.0f; }

	static ri::HTexture tex;
};


class Rockmelon : public Fruit {
public:
	Rockmelon() : Fruit() {
		if(tex.IsNull()) tex = ri::ServiceLocator::TexMgr().GetTexture("Rockmelon.png");

		SetTexture(tex);
		SetAlpha(true);
		SetOrigin(GetLocalBoundingBox().width / 2.0f, GetLocalBoundingBox().height / 2.0f);
		SetPosition(SPAWN_POS);
	}


	virtual type  GetType() const { return ROCKMELON; }
	virtual float GetHitboxWidth() const { return 83.525f; }

	static ri::HTexture tex;
};



class Bomb : public Fruit {
public:
	Bomb() : Fruit() {
		if(tex.IsNull()) tex = ri::ServiceLocator::TexMgr().GetTexture("Bomb.png");

		SetTexture(tex);
		SetAlpha(true);
		SetOrigin(GetLocalBoundingBox().width / 2.0f, GetLocalBoundingBox().height / 2.0f);
		SetPosition(SPAWN_POS);
	}


	virtual type  GetType() const { return BOMB; }
	virtual float GetHitboxWidth() const { return 90.0f; }

private:

	static ri::HTexture tex;
};


class Apple : public Fruit {
public:
	Apple() : Fruit() {
		if(tex.IsNull()) tex = ri::ServiceLocator::TexMgr().GetTexture("Apple.png");

		SetTexture(tex);
		SetAlpha(true);
		SetOrigin(GetLocalBoundingBox().width / 2.0f, GetLocalBoundingBox().height / 2.0f);
		SetPosition(SPAWN_POS);
	}


	virtual type  GetType() const { return APPLE; }
	virtual float GetHitboxWidth() const { return 45.0f; }

	static ri::HTexture tex;
};