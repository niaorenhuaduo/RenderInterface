#include "Spaceship.h"
#include "Menu.h"

bool Menu::OnInit()
{
	bgTex = ri::ServiceLocator::TexMgr().GetTexture("Menu.png");
	bg.SetTexture(bgTex);
	bg.SetOrigin(bg.GetLocalBoundingBox().width / 2.0f, bg.GetLocalBoundingBox().height / 2.0f);
	bg.SetPosition(static_cast<float>(ri::VIRTUALXRESOLUTION) / 2.0f, static_cast<float>(ri::VIRTUALYRESOLUTION) / 2.0f);

	mEasy.SetFont(gpPixelPop->mCash);
	mEasy.SetTextSize(100);
	mEasy.SetText(".Easy.");
	mEasy.SetOrigin(mEasy.GetLocalBoundingBox().width / 2.0f, 0.0f);
	mEasy.SetPosition(EASY_POS);
	mEasy.SetColor(ri::Color(152, 0, 0));

	mMedium.SetFont(gpPixelPop->mCash);
	mMedium.SetTextSize(100);
	mMedium.SetText("Medium");
	mMedium.SetOrigin(mMedium.GetLocalBoundingBox().width / 2.0f, 0.0f);
	mMedium.SetPosition(MIDDLE_POS);
	mMedium.SetColor(ri::Color(152, 0, 0));

	mHard.SetFont(gpPixelPop->mCash);
	mHard.SetTextSize(100);
	mHard.SetText("Hard");
	mHard.SetOrigin(mHard.GetLocalBoundingBox().width / 2.0f, 0.0f);
	mHard.SetPosition(HARD_POS);
	mHard.SetColor(ri::Color(152, 0, 0));

	mHelp.SetFont(gpPixelPop->mCash);
	mHelp.SetTextSize(100);
	mHelp.SetText("Help");
	mHelp.SetOrigin(mHelp.GetLocalBoundingBox().width / 2.0f, 0.0f);
	mHelp.SetPosition(static_cast<float>(ri::VIRTUALXRESOLUTION) / 2.0f, 618.0f);
	mHelp.SetColor(ri::Color::Black);


	mExit.SetFont(gpPixelPop->mCash);
	mExit.SetTextSize(100);
	mExit.SetText("Quit");
	mExit.SetOrigin(mExit.GetLocalBoundingBox().width / 2.0f, 0.0f);
	mExit.SetPosition(ri::VIRTUALXRESOLUTION / 2.0f, 796.0f);
	mExit.SetColor(ri::Color::Black);

	mHelpText.SetFont(gpPixelPop->mCash);
	mHelpText.SetTextSize(30);
	mHelpText.SetText("Chop the fruits to win the game! \nDifferent fruits require you to press different keys.\nWatermelon: \t\tSpacebar \nApple: \t\t\t\t   Right \nRockmelon: \t\t  Up \nListen to the beat to get the timing right, and avoid the Bombs!");
	ri::FloatRect helpTextRect = mHelpText.GetLocalBoundingBox();
	mHelpText.SetOrigin(helpTextRect.width / 2.0f, helpTextRect.height / 2.0f);
	mHelpText.SetPosition(ri::VIRTUALXRESOLUTION / 2.0f, ri::VIRTUALYRESOLUTION / 2.0f);
	mHelpText.SetColor(ri::Color::Black);

	mHelpBox.SetSize(helpTextRect.width + 10.0f, helpTextRect.height + 10.0f);
	mHelpBox.SetColor(ri::Color(255, 255, 255, 220));
	ri::FloatRect helpBoxRect = mHelpBox.GetLocalBoundingBox();
	mHelpBox.SetOrigin(helpBoxRect.width / 2.0f, helpBoxRect.height / 2.0f);
	mHelpBox.SetPosition(mHelpText.GetPosition());

	mbHelpBoxEnabled = false;

	mSelection = 0;
	mPreviousSelection = 0;

	gpPixelPop->GetAudio()->PlayCue(MENUMUSIC);

	return true;
}

void Menu::OnDestroy()
{
	ri::ServiceLocator::TexMgr().ReleaseTexture(bgTex);

	gpPixelPop->GetAudio()->StopCue(MENUMUSIC);
}

void Menu::OnUpdate( float fElapsedTime )
{
	if(mbHelpBoxEnabled) {
		if(gpPixelPop->GetInput()->AnyKeyPressed()) {
			mbHelpBoxEnabled = false;
			return;
		} else return;
	}

	if(gpPixelPop->GetInput()->IsKeyDown(VK_ESCAPE)) { mSelection = 4; return; }

	if(gpPixelPop->GetInput()->IsKeyDown(VK_SPACE) || gpPixelPop->GetInput()->IsKeyDown(VK_RETURN)) {
		Sleep(100);

		if(mSelection == 0) { gpPixelPop->VSetGameState(SpaceShip::GS_PLAYING_1); return; }
		else if(mSelection == 1) { gpPixelPop->VSetGameState(SpaceShip::GS_PLAYING_2); return; }
		else if(mSelection == 2) { gpPixelPop->VSetGameState(SpaceShip::GS_PLAYING_3); return; }
		else if(mSelection == 3) { mbHelpBoxEnabled = true; return; }
		else { gpPixelPop->ExitGame(); return; }
	}

	static bool bKeyPressedLastFrame = false;

	if(!bKeyPressedLastFrame) {
	bKeyPressedLastFrame = true;

	if(gpPixelPop->GetInput()->IsKeyDown(VK_DOWN)) {
		if(mSelection <= 2) {
			mSelection = 3;
		} else {
			++mSelection;
			if(mSelection > 4) mSelection = 0;
		}
	} else if(gpPixelPop->GetInput()->IsKeyDown(VK_UP)) {
		if(mSelection <= 2) {
			mSelection = 4;
		} else {
			--mSelection;
			if(mSelection == 2) mSelection = 0;
		}
	} else if(gpPixelPop->GetInput()->IsKeyDown(VK_RIGHT)) {
		if(mSelection <= 2) {
			++mSelection;
			if(mSelection > 2) mSelection = 0;
		}
	} else if(gpPixelPop->GetInput()->IsKeyDown(VK_LEFT)) {
		if(mSelection <= 2) {
			if(mSelection == 0) mSelection = 2;
			else --mSelection;
		}
	} else bKeyPressedLastFrame = false;

	}

	if(mSelection != mPreviousSelection) {
		gpPixelPop->GetAudio()->PlayCue(FLIP);

		ri::Label *pLabel = &mEasy;
		switch(mPreviousSelection) {
		case 0: mEasy.SetText("Easy");		pLabel = &mEasy; break;
		case 1: mMedium.SetText("Medium");	pLabel = &mMedium; break;
		case 2: mHard.SetText("Hard");		pLabel = &mHard; break;
		case 3: mHelp.SetText("Help");		pLabel = &mHelp; break;
		case 4: mExit.SetText("Quit");		pLabel = &mExit; break;
		}
		pLabel->SetOrigin(pLabel->GetLocalBoundingBox().width / 2.0f, 0.0f);
		
		switch(mSelection) {
		case 0: mEasy.SetText(".Easy.");		pLabel = &mEasy; break;
		case 1: mMedium.SetText(".Medium.");	pLabel = &mMedium; break;
		case 2: mHard.SetText(".Hard.");		pLabel = &mHard; break;
		case 3: mHelp.SetText(".Help.");		pLabel = &mHelp; break;
		case 4: mExit.SetText(".Quit.");		pLabel = &mExit; break;
		}
		pLabel->SetOrigin(pLabel->GetLocalBoundingBox().width / 2.0f, 0.0f);
	}

	mPreviousSelection = mSelection;
	if(!gpPixelPop->GetInput()->IsKeyDown(VK_UP) && !gpPixelPop->GetInput()->IsKeyDown(VK_DOWN) 
		&& !gpPixelPop->GetInput()->IsKeyDown(VK_RIGHT) && !gpPixelPop->GetInput()->IsKeyDown(VK_LEFT)) bKeyPressedLastFrame = false;
}

void Menu::OnRender()
{
	bg.OnDraw();

	mEasy.OnDraw();
	mMedium.OnDraw();
	mHard.OnDraw();

	mHelp.OnDraw();
	mExit.OnDraw();

	if(mbHelpBoxEnabled) {
		mHelpBox.OnDraw();
		mHelpText.OnDraw();
	}
}
