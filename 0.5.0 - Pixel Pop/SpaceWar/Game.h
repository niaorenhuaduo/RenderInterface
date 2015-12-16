#pragma once

#include <IRenderSystem.h>
#include "Input.h"
#include "Audio.h"
#include "GameError.h"


#ifdef _DEBUG
#pragma comment(lib, "RId.lib")
#else
#pragma comment(lib, "RI.lib")
#endif


class Game {
public:
	Game();
	virtual ~Game();

	virtual bool VOnInit(const std::string &optionsfile, HINSTANCE hInstance, int nCmdShow);
	virtual void	VRun();
	static LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// abstract methods
	virtual bool VOnInitialize()=0;
	virtual void VOnRender()=0;
	virtual void VOnUpdate()=0;
	virtual void VOnAI()=0;
	virtual void VOnCollisions()=0;
	virtual void VOnDestroy()=0;

	// Set/Get
	void ExitGame()		       { PostMessage(mhWnd, WM_DESTROY, 0, 0); }
	void SetPaused(bool pause) { mbPaused = pause; }

	Input*			   GetInput()      { return mpInput; }
	Audio*			   GetAudio()	   { return mpAudio; }
	bool			   IsInitialized() { return mbInitialized; }

protected:
	// Input
	Input		   *mpInput;

	// Sound
	Audio		   *mpAudio;

	// Windows
	HWND			mhWnd;
	HINSTANCE		mhInstance;

	// Timer
	LARGE_INTEGER	mTimeStart;
	LARGE_INTEGER	mTimeEnd; 
	LARGE_INTEGER	mTimerFreq;
	float mfElapsedTime;
	float mfFPS;
	DWORD mdwSleepTime;
	bool  mbPaused;

	bool					 mbInitialized;
	ri::RenderSystemSettings mOnLoadOptions;

private:
	void	LoadOptions(const std::string &optionsfile);
	bool	CreateMainWindow();
	
	virtual void VRenderGame();
};
extern Game *gpGame;