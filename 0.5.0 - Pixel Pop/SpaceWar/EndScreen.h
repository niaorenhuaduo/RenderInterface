#pragma once

#include <string>
#include <ServiceLocator.h>
#include <Drawable/Sprite.h>
#include <Drawable/Label.h>

class EndScreen {
public:
	EndScreen() {

	}


	bool OnInit(bool bWon);
	void OnDestroy();

	void OnUpdate(float fElapsedTime);
	void OnRender();

private:
	ri::Sprite bg;

	ri::HTexture win;
	ri::HTexture lost;

	ri::Label result;
	ri::Label text;
};