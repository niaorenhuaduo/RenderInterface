#pragma once

#include <RIRenderer.h>
#include "Input.h"
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

	virtual HRESULT VOnInit(HINSTANCE hInstance, int nCmdShow);
	virtual void	VRun();
	static LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// abstract methods
	virtual HRESULT VOnInitialize()=0;
	virtual void VOnRender()=0;
	virtual void VOnUpdate()=0;
	virtual void VOnAI()=0;
	virtual void VOnCollisions()=0;

	// Set/Get
	void ExitGame()		       { PostMessage(mhWnd, WM_DESTROY, 0, 0); }
	void SetPaused(bool pause) { mbPaused = pause; }

	PRIRENDERINTERFACE GetRenderer()   { return mpDevice; }
	Input*			   GetInput()      { return mpInput; }
	bool			   IsInitialized() { return mbInitialized; }

protected:
	// Renderer
	PRIRENDERER			mpRenderer;
	PRIRENDERINTERFACE  mpDevice;
	Input			   *mpInput;

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

	bool  mbInitialized;

private:
	HRESULT CreateMainWindow();
	
	virtual void VRenderGame();
};
extern Game *gpGame;