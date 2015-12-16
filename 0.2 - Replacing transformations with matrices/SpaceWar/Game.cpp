#include "Game.h"
#include "Constants.h"
#include <Mmsystem.h>
#include <RIServiceLocator.h>

Game::Game() : mhWnd(NULL), mhInstance(NULL), mpRenderer(NULL), mpDevice(NULL),
	mdwSleepTime(0), mfElapsedTime(1/30.0f), mfFPS(1000.0f), mbPaused(false), mbInitialized(false) {
	mpInput = new Input();
}

Game::~Game() {
	SAFE_DELETE(mpInput);
	SAFE_DELETE(mpRenderer);
}

HRESULT Game::VOnInit(HINSTANCE hInstance, int nCmdShow) {
	// Load options file
	mpRenderer = rnew RIRenderer();
	if(!mpRenderer) return E_FAIL;

	HRESULT res = mpRenderer->CreateRenderInterface(RENDER_API, OPTIONS_FILE);
	if(FAILED(res)) return E_FAIL;

	mpDevice = mpRenderer->GetRenderInterface();
	if(!mpDevice) return E_FAIL;

	// Create window
	mhInstance = hInstance;
	WNDCLASSEX wcx;
	wcx.style = CS_HREDRAW | CS_VREDRAW;
	wcx.cbSize = sizeof(wcx);
	wcx.lpfnWndProc = Game::MsgProc;
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = 0;
	wcx.hInstance = hInstance;
	wcx.hIcon = NULL;
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcx.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcx.lpszMenuName = NULL;
	wcx.lpszClassName = CLASS_NAME;
	wcx.hIconSm = NULL;
	if(RegisterClassEx(&wcx) == 0) return E_FAIL;											/* ERROR HANDLING */

	DWORD windowStyle;
	if(mpDevice->IsWindowed()) windowStyle = WS_OVERLAPPEDWINDOW;
	else windowStyle = WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP;

	U32 windowWidth = mpDevice->GetWindowWidth();
	U32 windowHeight = mpDevice->GetWindowHeight();

	mhWnd = CreateWindow(CLASS_NAME, APP_TITLE, windowStyle, CW_USEDEFAULT, CW_USEDEFAULT,
		windowWidth, windowHeight, NULL, NULL, hInstance, NULL);
	if(!mhWnd) return E_FAIL;																/* ERROR HANDLING */

	if(mpDevice->IsWindowed()) {    // Correct the client window size in windowed mode
		RECT clientRect;
		GetClientRect(mhWnd, &clientRect);
		MoveWindow(mhWnd, 0, 0, windowWidth+(windowWidth-clientRect.right), 
								windowHeight+(windowHeight-clientRect.bottom), TRUE);
	}

	ShowWindow(mhWnd, nCmdShow);
	UpdateWindow(mhWnd);

	// Create Input system
	mpInput->OnInit(mhWnd, CAPTURE_MOUSE);

	// Create Render Device
	if(FAILED(mpDevice->VOnInit(mhWnd))) return E_FAIL;
	RIServiceLocator::ProvideTextureManager(mpDevice->GetTexMgr());

	// Create Timer
	if(!QueryPerformanceFrequency(&mTimerFreq)) {
		MessageBox(NULL, L"High-definition counter not supported!", L"Error", MB_OK);
		return E_FAIL;
	}
	QueryPerformanceCounter(&mTimeStart);

	mbPaused = false;
	mbInitialized = true;

	if(FAILED(VOnInitialize())) return E_FAIL;

	return S_OK;
}



LRESULT CALLBACK Game::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	assert(gpGame);
	if(gpGame->IsInitialized()) {
		Input *pInput = gpGame->GetInput();

		switch(msg) {
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

		case WM_KEYDOWN: case WM_SYSKEYDOWN:
			pInput->KeyDown(wParam);
			return 0;
		case WM_KEYUP: case WM_SYSKEYUP:
			pInput->KeyUp(wParam);
			return 0;
		case WM_CHAR:
			pInput->keyIn(wParam);
			return 0;
		case WM_MOUSEMOVE:
			pInput->MouseIn(lParam);
			return 0;
		case WM_INPUT:
			pInput->MouseRawIn(lParam);
			return 0;
		case WM_LBUTTONDOWN:
			pInput->SetMouseLButton(true);
			pInput->MouseIn(lParam);
			return 0;
		case WM_LBUTTONUP:
			pInput->SetMouseLButton(false);
			pInput->MouseIn(lParam);
			return 0;	
		case WM_MBUTTONDOWN:
			pInput->SetMouseMButton(true);
			pInput->MouseIn(lParam);
			return 0;
		case WM_MBUTTONUP:
			pInput->SetMouseMButton(false);
			pInput->MouseIn(lParam);
			return 0;
		case WM_RBUTTONDOWN:
			pInput->SetMouseRButton(true);
			pInput->MouseIn(lParam);
			return 0;
		case WM_RBUTTONUP:
			pInput->SetMouseRButton(false);
			pInput->MouseIn(lParam);
			return 0;
		case WM_XBUTTONDOWN: case WM_XBUTTONUP:
			pInput->SetMouseXButton(wParam);
			pInput->MouseIn(lParam);
			return 0;
		case WM_DEVICECHANGE:
			pInput->CheckControllers();
			return 0;
		}
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void Game::VRun()
{
	assert(mbInitialized);

	QueryPerformanceCounter(&mTimeEnd);
	mfElapsedTime = (float)(mTimeEnd.QuadPart - mTimeStart.QuadPart) / (float)(mTimerFreq.QuadPart);

	// Power saving code
	if(mfElapsedTime < MIN_ELAPSED_TIME) {
		mdwSleepTime = (DWORD)((MIN_ELAPSED_TIME - mfElapsedTime) * 1000);
		timeBeginPeriod(1);
		Sleep(mdwSleepTime);
		timeEndPeriod(1);
		return;
	}

	if(mfElapsedTime > 0.0f) mfFPS = (mfFPS * 0.99f) + (0.01f/mfElapsedTime); // avg fps
	if(mfElapsedTime > MAX_FRAMERATE) mfElapsedTime = MAX_FRAMERATE;
	mTimeStart = mTimeEnd;
	mpInput->ReadControllers();

	if(!mbPaused) {
		VOnUpdate();
		VOnAI();
		VOnCollisions();
	}

	VRenderGame();

	// clear pressed buttons																		
	mpInput->Clear(Input::KEYS_PRESSED);
}

void Game::VRenderGame()
{
	if(SUCCEEDED(mpDevice->VBeginRendering())) {
		mpDevice->VClear(true, true, true);

		VOnRender();

		mpDevice->VEndRendering();
	}
}