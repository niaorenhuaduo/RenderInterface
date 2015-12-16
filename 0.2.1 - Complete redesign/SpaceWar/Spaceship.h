#pragma once

#include "Game.h"
#include <IRenderSystem.h>
#include <ServiceLocator.h>
#include <Sprite.h>

const float mfShipMinXPosition = 1.0f / 4.0f * ri::VIRTUALXRESOLUTION;
const float mfShipMaxXPosition = 3.0f / 4.0f * ri::VIRTUALXRESOLUTION;
const float mfMinMaxDistance = mfShipMaxXPosition - mfShipMinXPosition;
const float mfShipYPosition = ri::VIRTUALYRESOLUTION / 2.0f;
const float mfShipMaxScale = 3.0f;
const float mfShipMinScale = 0.5f;
const float mfMaxSpeed = 200.0f;
const float mfMinSpeed = 20.0f;

class SpaceShip : public Game {
public:
	HRESULT VOnInitialize() {

		mNebulaTexture = ri::ServiceLocator::TexMgr().VGetTexture("orion.jpg");
		mPlanetTexture = ri::ServiceLocator::TexMgr().VGetTexture("planet.dds");
		mShipTexture = ri::ServiceLocator::TexMgr().VGetTexture("ship.dds");


		mNebula.SetTexture(mNebulaTexture);
		
		mPlanet.SetTexture(mPlanetTexture);
		mPlanet.SetAlpha(true);
		ri::FloatRect planetrect = mPlanet.GetLocalBoundingBox();
		mPlanet.SetPosition(ri::VIRTUALXRESOLUTION / 2.0f, ri::VIRTUALYRESOLUTION / 2.0f);
		mPlanet.SetOrigin(planetrect.width / 2.0f, planetrect.height / 2.0f);


		mShip1 = rnew ri::Sprite(mShipTexture, 32, 32, 2, 0, 3, 0.25f, true);
		mShip1->SetOrigin(16.0f, 16.0f);
		mShip1->SetAlpha(true);
		mShip1->SetPosition(mfShipMinXPosition, mfShipYPosition);
		mfShip1Direction = 1.0f;

		mShip2 = rnew ri::Sprite(mShipTexture, 32, 32, 2, 0, 3, 0.125f, true);
		mShip2->SetOrigin(16.0f, 16.0f);
		mShip2->SetAlpha(true);
		mShip2->SetPosition(0.0f, 0.0f);
		mShip2Velocity.Set(0.0f, 0.0f);
		mShip2->SetRotation(90.0f);

		return S_OK;
	}

	void VOnDestroy() {
		SAFE_DELETE(mShip1);
		SAFE_DELETE(mShip2);
	}

	void VOnRender() {
		mNebula.OnDraw();

		if(mfShip1Direction > 0.0f) {
			mPlanet.OnDraw();
			mShip1->OnDraw();
		} else {
			mShip1->OnDraw();
			mPlanet.OnDraw();
		}
		mShip2->OnDraw();

	}

	void VOnUpdate() {
		//mPlanet.Rotate(-10.0f * mfElapsedTime);

		if(mpInput->IsKeyDown(VK_SPACE)) {
			ri::ServiceLocator::RenderSystem()._VSetYFlipping(false);
			ri::ServiceLocator::RenderSystem().VSetWindowed(true);
		}

		if(mpInput->IsKeyDown(VK_SHIFT)) {
			ri::ServiceLocator::RenderSystem()._VSetYFlipping(true);
			ri::ServiceLocator::RenderSystem().VSetWindowed(false);
		}

		mShip1->OnUpdate(mfElapsedTime);
		mShip2->OnUpdate(mfElapsedTime);

		mShip2Velocity.Set(0.0f, 0.0f);
		if(mpInput->IsKeyDown(VK_RIGHT)) {
			mShip2Velocity.x += 1.0f;
		}
		if(mpInput->IsKeyDown(VK_LEFT)) {
			mShip2Velocity.x -= 1.0f;
		}
		if(mpInput->IsKeyDown(VK_UP)) {
			mShip2Velocity.y -= 1.0f;
		}
		if(mpInput->IsKeyDown(VK_DOWN)) {
			mShip2Velocity.y += 1.0f;
		}

		const float speed = 200.0f;
		mShip2->SetPosition(mShip2->GetPosition() + mShip2Velocity * speed * mfElapsedTime);

		if(mShip2Velocity.GetLengthSquared() > 0) mShip2->SetRotation(ri::RadiansToDegree(mShip2Velocity.ToAngle()));


		// rotating ship
		float xPos = mShip1->GetPosition().x;
		float percent = (xPos - mfShipMinXPosition) / mfMinMaxDistance;

		float xVelocity = 0.0f;
		if(mfShip1Direction > 0.0f) {
			if(percent <= 0.5f) xVelocity = (percent + 0.5f)*mfMaxSpeed + (0.5f-percent)*mfMinSpeed;
			else				xVelocity = (-percent+1.5f)*mfMaxSpeed + (-0.5f+percent)*mfMinSpeed;
		} else {
			if(percent <= 0.5f) xVelocity = (percent + 0.5f)*mfMinSpeed + (0.5f-percent)*mfMaxSpeed;
			else				xVelocity = (-percent+1.5f)*mfMinSpeed + (-0.5f+percent)*mfMaxSpeed;
		}
		if(xPos > mfShipMaxXPosition) mfShip1Direction = -1.0f;
		if(xPos < mfShipMinXPosition) mfShip1Direction = 1.0f;

		float scale = 1.0f;
		if(mfShip1Direction > 0.0f) {
			if(percent <= 0.5f) scale = (percent + 0.5f)*mfShipMaxScale + (0.5f-percent)*mfShipMinScale;
			else				scale = (-percent+1.5f)*mfShipMaxScale + (-0.5f+percent)*mfShipMinScale;
		} else {
			if(percent <= 0.5f) scale = (percent + 0.5f)*mfShipMinScale + (0.5f-percent)*mfShipMaxScale;
			else				scale = (-percent+1.5f)*mfShipMinScale + (-0.5f+percent)*mfShipMaxScale;
		}

		mShip1->SetPosition(xPos += xVelocity * mfShip1Direction * mfElapsedTime, mfShipYPosition);
		mShip1->SetScaling(scale);

	}

	void VOnAI() {}
	void VOnCollisions() {}

private:
	ri::Vec2 mShip2Velocity;
	ri::Vec2 mShip1Position;
	float  mfShip1Angle;
	float  mfShip1Direction;

	ri::HTexture mNebulaTexture;
	ri::HTexture mPlanetTexture;
	ri::HTexture mShipTexture;

	ri::Sprite mNebula;
	ri::Sprite mPlanet;
	ri::Sprite *mShip1;
	ri::Sprite *mShip2;
};