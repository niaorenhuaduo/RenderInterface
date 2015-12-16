#pragma once

#include "Game.h"
#include <RISprite.h>

const float mfShipMinXPosition = 1.0f / 4.0f * RIVIRTUALXRESOLUTION;
const float mfShipMaxXPosition = 3.0f / 4.0f * RIVIRTUALXRESOLUTION;
const float mfMinMaxDistance = mfShipMaxXPosition - mfShipMinXPosition;
const float mfShipYPosition = RIVIRTUALYRESOLUTION / 2.0f;
const float mfShipMaxScale = 3.0f;
const float mfShipMinScale = 0.5f;
const float mfMaxSpeed = 200.0f;
const float mfMinSpeed = 20.0f;

class SpaceShip : public Game {
public:
	HRESULT VOnInitialize() {
		mNebulaTexture = mpDevice->GetTexMgr()->VGetTexture("orion.jpg");
		mPlanetTexture = mpDevice->GetTexMgr()->VGetTexture("planet.dds");
		mShipTexture = mpDevice->GetTexMgr()->VGetTexture("ship.dds");


		mNebula.SetTexture(mNebulaTexture);
		mNebula.SetFlippingVertical(true);
		
		mPlanet.SetTexture(mPlanetTexture);
		mPlanet.SetAlpha(true);
		RIFloatRect planetrect = mPlanet.GetLocalBoundingBox();
		mPlanet.SetPosition(RIVIRTUALXRESOLUTION / 2.0f, RIVIRTUALYRESOLUTION / 2.0f);
		mPlanet.SetOrigin(planetrect.width / 2.0f, planetrect.height / 2.0f);

		mShip1 = rnew RISprite(mShipTexture, 32, 32, 2, 0, 3, 0.25f, true);
		mShip1->SetOrigin(16.0f, 16.0f);
		mShip1->SetAlpha(true);
		mShip1->SetPosition(mfShipMinXPosition, mfShipYPosition);
		mfShip1Direction = 1.0f;

		mShip2 = rnew RISprite(mShipTexture, 32, 32, 2, 0, 3, 0.125f, true);
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
		mNebula.OnDraw(mpDevice);

		if(mfShip1Direction > 0.0f) {
			mPlanet.OnDraw(mpDevice);
			mShip1->OnDraw(mpDevice);
		} else {
			mShip1->OnDraw(mpDevice);
			mPlanet.OnDraw(mpDevice);
		}
		mShip2->OnDraw(mpDevice);

	}

	void VOnUpdate() {
		//mPlanet.Rotate(-10.0f * mfElapsedTime);

		if(mpInput->IsKeyDown(VK_SPACE)) {
			mShip2->SetScaling(0.2f);
		}

		if(mpInput->IsKeyDown(VK_SHIFT)) {
			mShip2->SetFlippingVertical(!mShip2->GetFlippingVertical());
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

		if(mShip2Velocity.GetLengthSquared() > 0) mShip2->SetRotation(RIRadiansToDegree(mShip2Velocity.ToAngle()));


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
	RIVec2 mShip2Velocity;
	RIVec2 mShip1Position;
	float  mfShip1Angle;
	float  mfShip1Direction;

	HTexture mNebulaTexture;
	HTexture mPlanetTexture;
	HTexture mShipTexture;

	RISprite mNebula;
	RISprite mPlanet;
	RISprite *mShip1;
	RISprite *mShip2;
};