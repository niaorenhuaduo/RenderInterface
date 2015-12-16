#pragma once

#include <Drawable/Sprite.h>
#include <Drawable/RectangleShape.h>
#include <ServiceLocator.h>


class Menu {
public:
	Menu() {
	}


	bool OnInit();
	void OnDestroy();

	void OnUpdate(float fElapsedTime);
	void OnRender();

private:
	ri::HTexture bgTex;
	ri::Sprite   bg;

	ri::Label mHelp;
	ri::Label mExit;

	ri::Label mEasy;
	ri::Label mMedium;
	ri::Label mHard;

	ri::Label			mHelpText;
	ri::RectangleShape  mHelpBox;
	bool				mbHelpBoxEnabled;

	U32 mSelection;
	U32 mPreviousSelection;
};