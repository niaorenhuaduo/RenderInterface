
#include "Constants.h"
#include "Spaceship.h"

bool	CheckForAnotherInstance();

HINSTANCE hInstance;
HWND	  hWnd;
Game     *gpGame = NULL;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine, int nCmdShow) {
	if(CheckForAnotherInstance()) return 0;

	try {
		gpGame = rnew SpaceShip();
		gpGame->VOnInit("options.txt", hInstance, nCmdShow);

		MSG msg;
		bool done = 0;
		while(!done) {
			while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
				if(msg.message == WM_QUIT) done = true;

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			gpGame->VRun();
		}
		gpGame->VOnDestroy();

		SAFE_DELETE(gpGame);

		return msg.wParam;
	} catch(const GameError &err) {
		DestroyWindow(hWnd);
		MessageBoxA(NULL, err.getMessage(), "ERROR", MB_OK);
	} catch(...) {
		DestroyWindow(hWnd);
		MessageBoxA(NULL, "Unknown Error occured.", "ERROR", MB_OK);
	}

	SAFE_DELETE(gpGame);
	return 0;
}

bool CheckForAnotherInstance() {
	HANDLE mutex = CreateMutex(NULL, true, APP_TITLE);
	if(GetLastError() == ERROR_ALREADY_EXISTS)
		return true;
	return false;
}