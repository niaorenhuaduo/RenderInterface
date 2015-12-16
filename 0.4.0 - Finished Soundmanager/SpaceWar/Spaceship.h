#pragma once

#include "Game.h"
#include <IRenderSystem.h>
#include <ServiceLocator.h>
#include <Drawable\Sprite.h>
#include <Camera.h>
#include <RenderState.h>
#include <Image.h>
#include <Drawable\Label.h>
#include <Drawable\RectangleShape.h>

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
		/********************************************************************/
		// Initialize Sprites
		/********************************************************************/

		// Load the textures
		mNebulaTexture = ri::ServiceLocator::TexMgr().GetTexture("space.jpg");
		mPlanetTexture = ri::ServiceLocator::TexMgr().GetTexture("bluePlanet.dds");
		mShipTexture = ri::ServiceLocator::TexMgr().GetTexture("ship.dds");

		// Nebula
		mNebula.SetTexture(mNebulaTexture);

		// Planet
		mPlanet.SetTexture(mPlanetTexture);
		mPlanet.SetAlpha(true);
		ri::FloatRect planetrect = mPlanet.GetLocalBoundingBox();
		mPlanet.SetPosition(ri::VIRTUALXRESOLUTION / 2.0f, ri::VIRTUALYRESOLUTION / 2.0f);
		mPlanet.SetOrigin(planetrect.width / 2.0f, planetrect.height / 2.0f);

		// Ship
		mShip1 = rnew ri::Sprite(mShipTexture, 32, 32, 2, 0, 3, 0.25f, true);
		mShip1->SetOrigin(16.0f, 16.0f);
		mShip1->SetAlpha(true);
		mShip1->SetPosition(mfShipMinXPosition, mfShipYPosition);


		/********************************************************************/
		// Initialize labels
		/********************************************************************/
		mArial.LoadFromFile("Arial.ttf");
		mImpact.LoadFromFile("orange.ttf");
		mMonster.LoadFromFile("monster.ttf");

		mpText = rnew ri::Label("Press 'd' for applause!", mMonster, 64);
		mpText->SetColor(ri::Color(0, 200, 0));
		mpText->SetShadowed(true, ri::Color(80, 0, 0));
		mpText->SetPosition(mShip1->GetPosition() - ri::Vec2(0.0f, mShip1->GetLocalBoundingBox().height + mpText->GetLocalBoundingBox().height));

		mpText2 = rnew ri::Label("Press 'a' for splash sound!", mMonster, 64);
		mpText2->SetColor(ri::Color(0, 0, 200));
		mpText2->SetShadowed(true, ri::Color(80, 0, 0));
		ri::FloatRect rect = mShip1->GetLocalBoundingBox();
		mpText2->SetPosition(mShip1->GetPosition() + ri::Vec2(0.0f, mShip1->GetLocalBoundingBox().height));


		// fps
		mpFPS = rnew ri::Label("FPS: 99.00", mImpact, 128);
		mpFPS->SetBold(true);
		mpFPS->SetShadowed(true, ri::Color::Black, ri::Vec2(128.0f / 30.0f));
		mpFPS->SetColor(ri::Color(255, 215, 0));
		ri::FloatRect fpsRect = mpFPS->GetLocalBoundingBox();
		mpFPS->SetPosition(ri::VIRTUALXRESOLUTION - fpsRect.width - fpsRect.left - 5.0f, 5.0f);

		// fps background
		mpFPSShape = rnew ri::RectangleShape(fpsRect.width, fpsRect.height, ri::Color(255, 255, 255, 80));


		// Play song
		mpAudio->PlayCue(SONG);

		return S_OK;
	}

	void VOnDestroy() {
		SAFE_DELETE(mShip1);
		SAFE_DELETE(mpText);
		SAFE_DELETE(mpText2);
		SAFE_DELETE(mpFPS);
		SAFE_DELETE(mpFPSShape);
	}

	void VOnRender() {
		ri::ServiceLocator::RenderSystem().Clear(true, true, true);

		/********************************************************************/
		// Draw Sprites
		/********************************************************************/
		mNebula.OnDraw();
		mPlanet.OnDraw();
		mShip1->OnDraw();


		/********************************************************************/
		// Draw Labels
		/********************************************************************/
		mpText->OnDraw();
		mpText2->OnDraw();

		mpFPSShape->OnDraw();
		mpFPS->OnDraw();
	}

	void VOnUpdate() {
		/********************************************************************/
		// Update Sprites
		/********************************************************************/
		mShip1->OnUpdate(mfElapsedTime);

		const float speed = 200.0f;
		mShip1Velocity.Set(0.0f, 0.0f);
		if(mpInput->IsKeyDown(VK_RIGHT)) {
			mShip1Velocity.x += 1.0f;
			mpText->AddToPosition(speed * mfElapsedTime, 0.0f);
			mpText2->AddToPosition(speed * mfElapsedTime, 0.0f);
		}
		if(mpInput->IsKeyDown(VK_LEFT)) {
			mShip1Velocity.x -= 1.0f;
			mpText->AddToPosition(-speed * mfElapsedTime, 0.0f);
			mpText2->AddToPosition(-speed * mfElapsedTime, 0.0f);
		}
		if(mpInput->IsKeyDown(VK_UP)) {
			mShip1Velocity.y -= 1.0f;
			mpText->AddToPosition(0.0f, -speed * mfElapsedTime);
			mpText2->AddToPosition(0.0f, -speed * mfElapsedTime);
		}
		if(mpInput->IsKeyDown(VK_DOWN)) {
			mShip1Velocity.y += 1.0f;
			mpText->AddToPosition(0.0f, speed * mfElapsedTime);
			mpText2->AddToPosition(0.0f, speed * mfElapsedTime);
		}

		static bool keyDPressedLastFrame = false;
		static bool keyAPressedLastFrame = false;
		if(mpInput->IsKeyDown('A') && !keyAPressedLastFrame) {
			mpAudio->PlayCue(SPLASH);
		}

		if(mpInput->IsKeyDown('D') && !keyDPressedLastFrame) {
			mpAudio->PlayCue(APPLAUSE);
		}

		mShip1->SetPosition(mShip1->GetPosition() + mShip1Velocity * speed * mfElapsedTime);
		if(mShip1Velocity.GetLengthSquared() > 0) mShip1->SetRotation(ri::RadiansToDegree(mShip1Velocity.ToAngle()));


		/********************************************************************/
		// Update Labels
		/********************************************************************/
		static char chFPS[64] = "infinite";
		if(mfElapsedTime > 0.0f) sprintf(chFPS, "FPS: %.2f", 1.0f / mfElapsedTime);
		else					 sprintf(chFPS, "FPS: infinite");

		static float fTextUpdateCooldown = 0.0f;
		fTextUpdateCooldown += mfElapsedTime;
		if(fTextUpdateCooldown >= 1.0f) {
			mpFPS->SetText(chFPS);
			fTextUpdateCooldown -= 1.0f;
		}

		ri::FloatRect fpsRect = mpFPS->GetLocalBoundingBox();
		mpFPSShape->SetSize(fpsRect.width, fpsRect.height);
		mpFPSShape->SetPosition(mpFPS->GetPosition() + ri::Vec2(fpsRect.left, fpsRect.top));

		static float counter = 0.0f;
		counter += mfElapsedTime;
		if(counter > 2.0f * ri::PI) counter -= 2.0f * ri::PI;

		float cosinus = cosf(counter) * 0.5f + 0.5f;
		float sinus   = sinf(counter) * 0.5f + 0.5f;
		float cosinus2 = cosf(counter * 2.0f) * 0.5f + 0.5f;
		float sinus2   = sinf(counter * 2.0f) * 0.5f + 0.5f;
		mpText->SetColor(ri::Color(sinus2 * 255, sinus2 * 255, sinus2 * 255));
		mpText2->SetColor(ri::Color(cosinus * 128, sinus * 255, cosinus2 * 255));



		keyAPressedLastFrame = mpInput->IsKeyDown('A');
		keyDPressedLastFrame = mpInput->IsKeyDown('D');
	}

	void VOnAI() {}
	void VOnCollisions() {}

private:
	ri::Vec2 mShip1Velocity;

	ri::HTexture mNebulaTexture;
	ri::HTexture mPlanetTexture;
	ri::HTexture mShipTexture;

	ri::Sprite mNebula;
	ri::Sprite mPlanet;
	ri::Sprite *mShip1;

	ri::Font mArial;
	ri::Font mImpact;
	ri::Font mMonster;
	ri::Label *mpText;
	ri::Label *mpText2;
	ri::Label *mpFPS;

	ri::RectangleShape *mpFPSShape;
};