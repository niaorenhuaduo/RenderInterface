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



#include "Menu.h"
#include "Playing.h"
#include "EndScreen.h"


class SpaceShip : public Game {
public:

	enum GameState {
		GS_NONE,
		GS_MENU,
		GS_PLAYING_1,
		GS_PLAYING_2,
		GS_PLAYING_3,
		GS_ENDSCREEN_WON,
		GS_ENDSCREEN_LOST
	};

	bool VOnInitialize() {
		mpMenu		= rnew Menu;
		mpPlaying	= rnew Playing;
		mpEndScreen = rnew EndScreen;
		
		mActState = GS_NONE;


		/********************************************************************/
		// Initialize labels
		/********************************************************************/
		mArial.LoadFromFile("Arial.ttf");
		mCash.LoadFromFile("CashCurrency.ttf");


		VSetGameState(GS_MENU);

		return true;
	}

	void VOnDestroy() {
		SAFE_DELETE(mpMenu);
		SAFE_DELETE(mpPlaying);
		SAFE_DELETE(mpEndScreen);

		mActState = GS_NONE;
	}

	void VOnRender() {
		ri::ServiceLocator::RenderSystem().Clear(true, true, true);

		switch(mActState) {
		case GS_MENU:											 mpMenu->OnRender();		break;
		case GS_PLAYING_1: case GS_PLAYING_2: case GS_PLAYING_3: mpPlaying->OnRender();		break;
		case GS_ENDSCREEN_WON: case GS_ENDSCREEN_LOST:			 mpEndScreen->OnRender();	break;

		}
	}

	void VOnUpdate() {

		switch(mActState) {
		case GS_MENU:											 mpMenu->OnUpdate(mfElapsedTime);		break;
		case GS_PLAYING_1: case GS_PLAYING_2: case GS_PLAYING_3: mpPlaying->OnUpdate(mfElapsedTime);	break;
		case GS_ENDSCREEN_WON: case GS_ENDSCREEN_LOST:			 mpEndScreen->OnUpdate(mfElapsedTime);	break;
		}

	}

	void VSetGameState(GameState newState) {
		if(newState == mActState) return;

		switch(mActState) {
		case GS_MENU:											 mpMenu->OnDestroy();		break;
		case GS_PLAYING_1: case GS_PLAYING_2: case GS_PLAYING_3: mpPlaying->OnDestroy();	break;
		case GS_ENDSCREEN_WON: case GS_ENDSCREEN_LOST:			 mpEndScreen->OnDestroy();	break;
		}


		switch(newState) {
		case GS_MENU:			mpMenu->OnInit();				break;
		case GS_PLAYING_1:		mpPlaying->OnInit(1);			break;
		case GS_PLAYING_2:		mpPlaying->OnInit(2);			break;
		case GS_PLAYING_3:		mpPlaying->OnInit(3);			break;
		case GS_ENDSCREEN_WON:	mpEndScreen->OnInit(true);		break;
		case GS_ENDSCREEN_LOST:	mpEndScreen->OnInit(false);		break;		
		}

		mActState = newState;
	}

	void VOnAI() {}
	void VOnCollisions() {}

	// Fonts are public
	ri::Font mArial;
	ri::Font mCash;

private:

	Menu		*mpMenu;
	Playing		*mpPlaying;
	EndScreen	*mpEndScreen;

	GameState mActState;
};

extern SpaceShip *gpPixelPop;