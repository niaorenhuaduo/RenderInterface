#pragma once

#include <RI.h>
#include <RIRenderer.h>
#include <RIRenderInterface.h>
#include "..\Utilities\Clock.h"
#include "..\Events\EventManagerImpl.h"													

// TODO: Application::Pause: Post pause event
// TODO: Timer in mainLoop
// TODO: Input handling

class GameOptions;


class Application : private UnCopyable {
public:
	Application();
	virtual ~Application();

	HRESULT OnInit(const HINSTANCE hInstance, const char* const pOptions);
	int		Go();	// MainLoop

	static LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);	// Handles input 

	// Language file
	HRESULT		 LoadLanguage(const std::string &language);
	std::wstring GetString(const std::wstring &textID) const;

	virtual const WCHAR* VGetGameTitle() const=0;
	virtual const HICON  VGetGameIcon() const=0;

	void	  Pause(bool bPause);
	void	  Abort()			  { mbQuitting = true; mpDevice->SetRunning(false); }
	HINSTANCE GetInstance() const { return mhInstance; }
	Clock*    GetClock() const	  { return mpClock; }

	virtual void VOnDestroy()=0;
	virtual void VOnInit()=0;	// DELETE
	virtual void VOnUpdate()=0;
protected:
	// Render-DLL
	PRIRENDERER		   mpRenderer;
	PRIRENDERINTERFACE mpDevice;

	// Language-data
	std::map<std::wstring, std::wstring> mText;

	// Windows-Stuff
	HWND		mhWnd;
	HINSTANCE	mhInstance;
	bool		mbInitialized;
	bool		mbQuitting;
	bool		mbPaused;
	RECT		mDesktopSize;

	GameOptions  *mpOptions;		 // GameOptions
	Clock		 *mpClock;
	EventManager *mpEventManager;


private:
	void	OnClose();
	void	OnUpdate();
	void	OnRender();
	void	OnAltEnter();
	HRESULT	OnDisplayChange(U32 colorDepth, U32 width, U32 height);

	HRESULT InitLogger();
	void	CloseLogger();

	HRESULT CreateMainWindow();
};
extern Application *gpApp;