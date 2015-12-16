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
		mArial.LoadFromFile("Arial.ttf");
		mImpact.LoadFromFile("orange.ttf");
		mMonster.LoadFromFile("monster.ttf");

		mArialTexture.SetTexture(mMonster.GetTexture(64));
		mArialTexture.SetPosition(0.0f, 0.0f);
		mArialTexture.SetAlpha(true);

		ri::FloatRect arialTexRect = mArialTexture.GetLocalBoundingBox();
		mpArialTextureBackground = rnew ri::RectangleShape(arialTexRect.width, arialTexRect.height, ri::Color(255, 255, 255, 128));

		// initialize labels
		mpText = rnew ri::Label("Press a button!", mMonster, 64);
		mpText->SetColor(ri::Color(0, 200, 0));
		mpText->SetShadowed(true, ri::Color(80, 0, 0));

		// rotating text/shape
		std::string text = "South Park ist eine US-amerikanische Animationsserie von Trey Parker\nund Matt Stone mit gesellschaftskritischem, humoristischem Inhalt.\nSeit 1997 läuft die Serie, von der bereits 18 Staffeln produziert\nwurden, auf dem US-Kabelsender Comedy Central. In Deutschland wurde South Park von 1999 bis 2006 auf RTL,\nvon 2003 bis 2005 auf VIVA von 2006 bis 2011 auf MTV sowie seit 2008 auf";
		mpRotatingText = rnew ri::Label(text, mArial, 16);
		mpRotatingText->SetShadowed(true);
		ri::FloatRect rotatingTextRect = mpRotatingText->GetLocalBoundingBox();
		mpRotatingText->SetOrigin(rotatingTextRect.width / 2.0f, rotatingTextRect.height / 2.0f);
		mpRotatingText->SetPosition(3.0f/4.0f * ri::VIRTUALXRESOLUTION, 3.0f / 4.0f * ri::VIRTUALYRESOLUTION);

		mpRotatingShape = rnew ri::RectangleShape(rotatingTextRect.width, rotatingTextRect.height, ri::Color(200, 200, 128, 128));

		// fps
		mpFPS = rnew ri::Label("FPS: 99.00", mImpact, 128);
		mpFPS->SetBold(true);
		mpFPS->SetShadowed(true, ri::Color::Black, ri::Vec2(128.0f / 30.0f));
		mpFPS->SetColor(ri::Color(255, 215, 0));
		ri::FloatRect fpsRect = mpFPS->GetLocalBoundingBox();
		mpFPS->SetPosition(ri::VIRTUALXRESOLUTION - fpsRect.width - fpsRect.left - 5.0f, 5.0f);

		// fps background
		mpFPSShape = rnew ri::RectangleShape(fpsRect.width, fpsRect.height, ri::Color(255, 255, 255, 80));

		mpSmiley = rnew ri::Label(":-)", mArial, 64);
		mpSmiley->Scale(1.5f);
		ri::FloatRect smileyRect = mpSmiley->GetLocalBoundingBox();
		mpSmiley->SetOrigin((smileyRect.left + smileyRect.width) / 2.0f, (smileyRect.top + smileyRect.height) / 2.0f);
		mpSmiley->SetPosition(smileyRect.width * 1.5f / 2.0f + 10.0f, smileyRect.height * 1.5f / 2.0f + 10.0f);


		mpTextureDescription = rnew ri::Label("This is the texture that holds the bitmap for the font:\nArial (64)", mArial, 32);
		mpTextureDescription->SetBold(true);
		mpTextureDescription->SetColor(ri::Color(200, 200, 200));

		ri::FloatRect texDescRect = mpTextureDescription->GetLocalBoundingBox();
		mpTextShape = rnew ri::RectangleShape(texDescRect.width, texDescRect.height, ri::Color(0, 50, 0, 128));

		// Load the textures
		mNebulaTexture = ri::ServiceLocator::TexMgr().GetTexture("space.jpg");
		mPlanetTexture = ri::ServiceLocator::TexMgr().GetTexture("bluePlanet.dds");
		mShipTexture = ri::ServiceLocator::TexMgr().GetTexture("ship.dds");


		// Initialize the sprites
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

		return S_OK;
	}

	void VOnDestroy() {
		SAFE_DELETE(mShip1);
		SAFE_DELETE(mpText);
		SAFE_DELETE(mpFPS);
		SAFE_DELETE(mpSmiley);
		SAFE_DELETE(mpTextureDescription);
		SAFE_DELETE(mpTextShape);
		SAFE_DELETE(mpFPSShape);
		SAFE_DELETE(mpRotatingText);
		SAFE_DELETE(mpRotatingShape);
		SAFE_DELETE(mpArialTextureBackground);
	}

	void VOnRender() {
		ri::ServiceLocator::RenderSystem().Clear(true, true, true);

		mNebula.OnDraw();

		mPlanet.OnDraw();
		mShip1->OnDraw();

		// rotating text/shape
		mpRotatingShape->OnDraw();
		mpRotatingText->OnDraw();

		mpText->OnDraw();

		ri::FloatRect fpsRect = mpFPS->GetLocalBoundingBox();
		mpFPSShape->SetSize(fpsRect.width, fpsRect.height);
		mpFPSShape->SetPosition(mpFPS->GetPosition() + ri::Vec2(fpsRect.left, fpsRect.top));
		mpFPSShape->OnDraw();
		mpFPS->OnDraw();

		mpSmiley->OnDraw();

		mArialTexture.SetTexture(mMonster.GetTexture(64));
		mArialTexture.SetPosition(5.0f, ri::VIRTUALYRESOLUTION - mArialTexture.GetLocalBoundingBox().height - 5.0f);
		mArialTexture.OnDraw();

		ri::FloatRect arialTexRect = mArialTexture.GetLocalBoundingBox();
		mpArialTextureBackground->SetSize(arialTexRect.width, arialTexRect.height);
		mpArialTextureBackground->SetPosition(mArialTexture.GetPosition());
		mpArialTextureBackground->OnDraw();


		// Texture Description
		ri::FloatRect texDescRect = mpTextureDescription->GetLocalBoundingBox();
		mpTextureDescription->SetPosition(5.0f, mArialTexture.GetPosition().y - texDescRect.height - texDescRect.top);

		mpTextShape->SetSize(texDescRect.width, texDescRect.height);
		mpTextShape->SetPosition(mpTextureDescription->GetPosition() + ri::Vec2(texDescRect.left, texDescRect.top));

		mpTextShape->OnDraw();
		mpTextureDescription->OnDraw();
	}

	void VOnUpdate() {
		// Update sprites
		mShip1->OnUpdate(mfElapsedTime);

		const float speed = 200.0f;
		mShip1Velocity.Set(0.0f, 0.0f);
		if(mpInput->IsKeyDown(VK_RIGHT)) {
			mShip1Velocity.x += 1.0f;
			mpText->AddToPosition(speed * mfElapsedTime, 0.0f);
		}
		if(mpInput->IsKeyDown(VK_LEFT)) {
			mShip1Velocity.x -= 1.0f;
			mpText->AddToPosition(-speed * mfElapsedTime, 0.0f);
		}
		if(mpInput->IsKeyDown(VK_UP)) {
			mShip1Velocity.y -= 1.0f;
			mpText->AddToPosition(0.0f, -speed * mfElapsedTime);
		}
		if(mpInput->IsKeyDown(VK_DOWN)) {
			mShip1Velocity.y += 1.0f;
			mpText->AddToPosition(0.0f, speed * mfElapsedTime);
		}


		mShip1->SetPosition(mShip1->GetPosition() + mShip1Velocity * speed * mfElapsedTime);
		if(mShip1Velocity.GetLengthSquared() > 0) mShip1->SetRotation(ri::RadiansToDegree(mShip1Velocity.ToAngle()));


		// Update labels
		mpSmiley->Rotate(45.0f * mfElapsedTime);
		static float colorIndex = 0.0f;
		colorIndex += mfElapsedTime; if(colorIndex >= ri::PI) colorIndex -= ri::PI;
		mpSmiley->SetColor(ri::Color(sinf(colorIndex) * 255.0f, 0, 0));

		mpRotatingText->Rotate(cosf(colorIndex * 2.0f) * 45.0f * mfElapsedTime);

		ri::FloatRect rotatingTextRect = mpRotatingText->GetGlobalBoundingBox();
		mpRotatingShape->SetSize(rotatingTextRect.width, rotatingTextRect.height);
		mpRotatingShape->SetPosition(rotatingTextRect.left, rotatingTextRect.top);


		std::string input = mpInput->GetTextIn();
		if(input.size() != 0) mpText->SetText(mpInput->GetTextIn());
		else				  mpText->SetText("Press a button!");
		ri::FloatRect textRect = mpText->GetLocalBoundingBox();
		mpText->SetOrigin((textRect.left + textRect.width) / 2.0f, (textRect.top + textRect.height) / 2.0f);
		mpText->SetPosition(1.0f / 3.0f * ri::VIRTUALXRESOLUTION, 1.0f / 3.0f * ri::VIRTUALYRESOLUTION / 2.0f);

		static char chFPS[64] = "infinite";
		if(mfElapsedTime > 0.0f) sprintf(chFPS, "FPS: %.2f", 1.0f / mfElapsedTime);
		else					 sprintf(chFPS, "FPS: infinite");

		static float fTextUpdateCooldown = 0.0f;
		fTextUpdateCooldown += mfElapsedTime;
		if(fTextUpdateCooldown >= 1.0f) {
			mpFPS->SetText(chFPS);
			fTextUpdateCooldown -= 1.0f;
		}

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
	ri::Label *mpSmiley;
	ri::Label *mpText;
	ri::Label *mpFPS;
	ri::Label *mpTextureDescription;
	ri::Label *mpRotatingText;
	ri::Sprite mArialTexture;

	ri::RectangleShape *mpTextShape;
	ri::RectangleShape *mpFPSShape;
	ri::RectangleShape *mpRotatingShape;
	ri::RectangleShape *mpArialTextureBackground;
};