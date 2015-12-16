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


		mNebula.VOnInit(mpDevice, mNebulaTexture, NULL, false);
		mNebula.SetWidthHeight(RIVIRTUALXRESOLUTION, RIVIRTUALYRESOLUTION);

		mPlanet.VOnInit(mpDevice, mPlanetTexture, NULL, true);
		mPlanet.SetPosition(RIVIRTUALXRESOLUTION / 2.0f - mPlanet.GetWidth()/2.0f, -mPlanet.GetHeight()/2.0f + RIVIRTUALYRESOLUTION / 2.0f);

		mShip1.VOnInit(mpDevice, mShipTexture, 32, 32, 2, 0, 3, 0.25f, true);
		mShip1.SetPosition(mfShipMinXPosition - mShip1.GetWidth()/2.0f, mfShipYPosition - mShip1.GetHeight()/2.0f);
		mfShip1Direction = 1.0f;

		mShip2.VOnInit(mpDevice, mShipTexture, 32, 32, 2, 0, 3, 0.125f, true);
		mShip2.SetPosition(0.0f, 0.0f - mShip2.GetHeight()/2.0f);
		mShip2Velocity.Set(0.0f, 0.0f);
		mShip2.SetRotationDegrees(90.0f);

		return S_OK;
	}

	void VOnRender() {
		mNebula.VOnDraw();
		if(mfShip1Direction > 0.0f) {
			mPlanet.VOnDraw();
			mShip1.VOnDraw();
		} else {
			mShip1.VOnDraw();
			mPlanet.VOnDraw();
		}
		mShip2.VOnDraw();

	}

	void VOnUpdate() {
		mShip1.VOnUpdate(mfElapsedTime);
		mShip2.VOnUpdate(mfElapsedTime);

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
		mShip2.SetPosition(mShip2.GetPosition() + mShip2Velocity * speed * mfElapsedTime);

		if(mShip2Velocity.GetLengthSquared() > 0) mShip2.SetRotationRad(mShip2Velocity.ToAngle());


		// rotating ship
		float xPos = mShip1.GetX();
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

		mShip1.SetX(xPos += xVelocity * mfShip1Direction * mfElapsedTime);
		mShip1.SetY(mfShipYPosition - mShip1.GetRelativeCenter().y); 
		mShip1.SetScale(scale);

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
	RISprite mShip1;
	RISprite mShip2;
};