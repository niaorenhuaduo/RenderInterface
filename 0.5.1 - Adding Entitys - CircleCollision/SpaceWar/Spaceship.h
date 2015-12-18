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
#include <Color.h>
#include <RIMath.h>

#include "Circle.h"


class SpaceShip : public Game {
public:

	bool VOnInitialize() {
		random.Randomize();

		/********************************************************************/
		// Initialize labels
		/********************************************************************/
		mArial.LoadFromFile("Arial.ttf");
		mCash.LoadFromFile("CashCurrency.ttf");

		ri::HTexture bgTex = ri::ServiceLocator::TexMgr().GetTexture("Dragon.jpg");
		ri::HTexture collisionTex = ri::ServiceLocator::TexMgr().GetTexture("CollisionTest.png");

		/********************************************************************/
		// Initialize sprites
		/********************************************************************/
		bg.SetTexture(bgTex);

		for(U32 i=0; i<mNumCircles; ++i) {
			mpCircles[i] = rnew Circle();
			mpCircles[i]->SetPosition(random.Random() * (ri::VIRTUALXRESOLUTION-100) + 50.0f, random.Random() * (ri::VIRTUALYRESOLUTION-100) + 50.0f);
			mpCircles[i]->SetVelocity(ri::Vec2(200 + random.Random() * 200, 200 + random.Random() * 200));

			float scaling = random.Random() + 0.5f;
			mpCircles[i]->Scale(scaling);
			mpCircles[i]->SetMass(mpCircles[i]->GetMass() * scaling);
			mpCircles[i]->SetColor(ri::Color(140 * scaling, 140 * scaling, 170 * scaling));
		}

		mpCircles[0]->SetPosition(static_cast<float>(ri::VIRTUALXRESOLUTION) / 2.0f, static_cast<float>(ri::VIRTUALYRESOLUTION) / 2.0f);
		mpCircles[0]->SetMass(10000000.0f);
		mpCircles[0]->SetColor(ri::Color(255, 0, 0));
		mpCircles[0]->SetVelocity(ri::Vec2(0.0f, 0.0f));

 		//mpCircles[1]->SetMass(10.0f);
 		//mpCircles[1]->SetVelocity(ri::Vec2(700.0f, 700.0f));
 		//mpCircles[1]->SetColor(ri::Color(128, 255, 128));
 
 		//mpCircles[2]->SetMass(0.5f);
 		//mpCircles[2]->SetVelocity(ri::Vec2(200.0f, 200.0f));
 		//mpCircles[2]->SetColor(ri::Color(128, 128, 255));



		return true;
	}

	void VOnDestroy() {
		for(U32 i=0; i<mNumCircles; ++i) {
			SAFE_DELETE(mpCircles[i]);
		}
	}

	void VOnRender() {
		bg.OnDraw();

		for(U32 i=0; i<mNumCircles; ++i) {
			mpCircles[i]->OnDraw();
		}
	}

	void VOnUpdate() {
		for(U32 i=0; i<mNumCircles; ++i) {
			mpCircles[i]->OnUpdate(mfElapsedTime);


		}

		for(U32 i=0; i<mNumCircles; ++i) {
			for(U32 j=i+1; j<mNumCircles; ++j) {

				ri::Vec2 colVec;
				if(mpCircles[i]->CollidesWith(*mpCircles[j], &colVec)) {
					mpCircles[i]->Bounce(-colVec, *mpCircles[j]);
					mpCircles[j]->Bounce(colVec, *mpCircles[i]);
				}


			}
		}

	}


	void VOnAI() {}
	void VOnCollisions() {}

	// Fonts are public
	ri::Font mArial;
	ri::Font mCash;

private:

	static const U32 mNumCircles = 16;
	Circle *mpCircles[mNumCircles];

	ri::Sprite bg;

	ri::RandomGenerator random;
};

extern SpaceShip *gpPixelPop;