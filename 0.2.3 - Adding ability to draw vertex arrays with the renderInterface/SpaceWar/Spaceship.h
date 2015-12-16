#pragma once

#include "Game.h"
#include <IRenderSystem.h>
#include <ServiceLocator.h>
#include <Sprite.h>
#include <Camera.h>
#include <RenderState.h>

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
		random.Randomize();

		mNebulaTexture = ri::ServiceLocator::TexMgr().GetTexture("orion.jpg");
		mPlanetTexture = ri::ServiceLocator::TexMgr().GetTexture("planet.dds");
		mShipTexture = ri::ServiceLocator::TexMgr().GetTexture("ship.dds");


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
		mShip2->SetPosition(static_cast<float>(ri::VIRTUALXRESOLUTION) / 2.0f, static_cast<float>(ri::VIRTUALYRESOLUTION) / 2.0f);
		mShip2Velocity.Set(0.0f, 0.0f);
		mShip2->SetRotation(90.0f);



		// Initialize vertices, stars background
		ri::Vertex2D vertices[6];
		quad[0].mPos = ri::Vec2(0.0f, 0.0f);
		quad[0].mColor = ri::Color(255, 255, 255);
		quad[0].mTex = ri::Vec2(0.0f, 0.0f);

		quad[1].mPos = ri::Vec2(1.0f, 0.0f);
		quad[1].mColor = ri::Color(255, 255, 255);
		quad[1].mTex = ri::Vec2(1.0f, 0.0f);

		quad[2].mPos = ri::Vec2(0.0f, 1.0f);
		quad[2].mColor = ri::Color(255, 255, 255);
		quad[2].mTex = ri::Vec2(0.0f, 1.0f);

		quad[3].mPos = ri::Vec2(0.0f, 1.0f);
		quad[3].mColor = ri::Color(255, 255, 255);
		quad[3].mTex = ri::Vec2(0.0f, 1.0f);

		quad[4].mPos = ri::Vec2(1.0f, 0.0f);
		quad[4].mColor = ri::Color(255, 255, 255);
		quad[4].mTex = ri::Vec2(1.0f, 0.0f);

		quad[5].mPos = ri::Vec2(1.0f, 1.0f);
		quad[5].mColor = ri::Color(255, 255, 255);
		quad[5].mTex = ri::Vec2(1.0f, 1.0f);


		// stars
		for(U32 i=0; i<300; ++i) {
			stars[i].mPos = ri::Vec2((random.Random() - 0.5f) * 120.0f, (random.Random() - 0.5f) * 120.0f);
			stars[i].mColor = ri::Color(random.Random(255), random.Random(255), random.Random(255));
		}

		for(U32 i=0; i<10; ++i) {
			lines[i].mPos = ri::Vec2((random.Random() * 0.5f) * ri::VIRTUALXRESOLUTION, random.Random() * ri::VIRTUALYRESOLUTION);
			lines[i].mColor = ri::Color(random.Random(255), random.Random(255), random.Random(255));

			connectedLines[i].mPos = ri::Vec2((random.Random() * 0.5f + 0.5f) * ri::VIRTUALXRESOLUTION, random.Random() * ri::VIRTUALYRESOLUTION);
			connectedLines[i].mColor = ri::Color(random.Random(255), random.Random(255), random.Random(255));
		}

		return S_OK;
	}

	void VOnDestroy() {
		SAFE_DELETE(mShip1);
		SAFE_DELETE(mShip2);
	}

	void VOnRender() {
		ri::ServiceLocator::RenderSystem().Clear(true, true, true);
		mNebula.OnDraw();

		// Render colored background
		ri::Matrix2D coloredbgMat;
		coloredbgMat.Translate(-0.5f, -0.5f);
		coloredbgMat.Scale(400.0f, 0.625f * 400.0f);
		coloredbgMat.Translate(ri::VIRTUALXRESOLUTION / 2.0f, ri::VIRTUALYRESOLUTION * 3.0f / 4.0f);
		quad[0].mColor = ri::Color(0, 255, 0);
		quad[5].mColor = ri::Color(255, 0, 0);
		ri::ServiceLocator::RenderSystem()._Draw(quad, 6, ri::PRIMITIVETYPE_TRIANGLE, ri::RenderState(coloredbgMat, mNebulaTexture, false));
		quad[0].mColor = quad[1].mColor = quad[2].mColor = quad[3].mColor = quad[4].mColor = quad[5].mColor = ri::Color(255, 255, 255);

		// Render stars background
		ri::Vec2 starsPosition(ri::VIRTUALXRESOLUTION / 2.0f, ri::VIRTUALYRESOLUTION / 4.0f);

		ri::Matrix2D starsbgMat;
		starsbgMat.Translate(-0.5f, -0.5f);
		starsbgMat.Scale(120.0f, 120.0f);
		starsbgMat.Translate(starsPosition); 
		ri::ServiceLocator::RenderSystem()._Draw(quad, 6, ri::PRIMITIVETYPE_TRIANGLE, ri::RenderState(starsbgMat));

		// Render stars
		ri::Matrix2D starsMat;
		starsMat.Translate(starsPosition);
		ri::ServiceLocator::RenderSystem()._Draw(stars, 300, ri::PRIMITIVETYPE_POINTS, ri::RenderState(starsMat));

		// Render lines
		ri::ServiceLocator::RenderSystem()._Draw(lines, 10, ri::PRIMITIVETYPE_LINES, ri::RenderState());
		ri::ServiceLocator::RenderSystem()._Draw(connectedLines, 10, ri::PRIMITIVETYPE_LINESTRIP, ri::RenderState());


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
		if(mpInput->IsKeyDown(VK_SPACE)) {
			ri::ServiceLocator::RenderSystem()._SetYFlipping(false);
			ri::ServiceLocator::RenderSystem().SetWindowed(true);
		}

		if(mpInput->IsKeyDown(VK_SHIFT)) {
			ri::ServiceLocator::RenderSystem()._SetYFlipping(true);
			ri::ServiceLocator::RenderSystem().SetWindowed(false);
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

	ri::RandomGenerator random;
	ri::Vertex2D quad[6];
	ri::Vertex2D stars[300]; 
	ri::Vertex2D lines[10];
	ri::Vertex2D connectedLines[10];

};