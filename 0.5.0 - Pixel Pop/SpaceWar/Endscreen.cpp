#include "EndScreen.h"
#include "Spaceship.h"

bool EndScreen::OnInit(bool bWon) {
	win = ri::ServiceLocator::TexMgr().GetTexture("win.png");
	lost = ri::ServiceLocator::TexMgr().GetTexture("lost.png");

	if(bWon) bg.SetTexture(win);
	else bg.SetTexture(lost);

	bg.SetOrigin(bg.GetLocalBoundingBox().width / 2.0f, bg.GetLocalBoundingBox().height / 2.0f);
	bg.SetPosition(ri::VIRTUALXRESOLUTION / 2.0f, ri::VIRTUALYRESOLUTION / 2.0f);

	result.SetFont(gpPixelPop->mCash);
	result.SetTextSize(70.0f);
	if(bWon) result.SetText("You Won!");
	else     result.SetText("You Lost!");
	result.SetOrigin(result.GetLocalBoundingBox().width / 2.0f, 0.0f);
	result.SetPosition(static_cast<float>(ri::VIRTUALXRESOLUTION) / 2.0f, static_cast<float>(ri::VIRTUALYRESOLUTION) / 2.0f + 250.0f);
	result.SetColor(ri::Color::Black);

	text.SetFont(gpPixelPop->mCash);
	text.SetTextSize(35.0f);
	text.SetText("Press any key to continue");
	text.SetOrigin(text.GetLocalBoundingBox().width / 2.0f, 0.0f);
	text.SetPosition(static_cast<float>(ri::VIRTUALXRESOLUTION) / 2.0f, static_cast<float>(ri::VIRTUALYRESOLUTION) / 2.0f + 330.0f);
	text.SetColor(ri::Color::Black);


	if(bWon) gpPixelPop->GetAudio()->PlayCue(WIN);
	else	 gpPixelPop->GetAudio()->PlayCue(LOST);

	return true;
}

void EndScreen::OnDestroy() {
	ri::ServiceLocator::TexMgr().ReleaseTexture(win);
	ri::ServiceLocator::TexMgr().ReleaseTexture(lost);

	gpPixelPop->GetAudio()->StopCue(WIN);
	gpPixelPop->GetAudio()->StopCue(LOST);
}

void EndScreen::OnUpdate(float fElapsedTime) {
	if(gpPixelPop->GetInput()->AnyKeyPressed()) gpPixelPop->VSetGameState(SpaceShip::GS_MENU);
}

void EndScreen::OnRender() {
	bg.OnDraw();

	result.OnDraw();
	text.OnDraw();
}