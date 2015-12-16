#include "..\Global.h"

#include "Application.h"

#include "..\Debugging\Logger.h"
#include "..\Mainloop\Initialization.h"

#include "..\Utilities\String.h"
#include "..\3rdParty\tinyxml2\tinyxml2.h"

Application *gpApp = NULL;


Application::Application() :
	mpRenderer(NULL),
	mpDevice(NULL), 
	mhWnd(NULL),
	mpOptions(NULL),
	mhInstance(NULL),
	mbInitialized(false),
	mbQuitting(false),
	mbPaused(false),
	mDesktopSize()
{
	gpApp = this;	
}

Application::~Application() {
	OnClose();
}

HRESULT Application::OnInit(const HINSTANCE hInstance, const char* const pOptions) {
	if(mbInitialized)
		return S_OK;
	assert(hInstance && pOptions);
	mhInstance = hInstance;

#ifndef _DEBUG
	if(!IsOnlyInstance(VGetGameTitle()))
		return E_FAIL;
#endif

	if(FAILED(InitLogger()))
		return E_FAIL;


	FLOG(logINFO) << "Initializing main-clock.";
	mpClock = rnew Clock();
	assert(mpClock && "Out of memory!");
	if(FAILED(mpClock->Init())) {
		FLOG(logERROR) << "Failed to initialize mainClock";
		return E_FAIL;
	}

	FLOG(logINFO) << "Loading Options from: " << pOptions;
	{
		mpOptions = rnew GameOptions();
		assert(mpOptions && "Out of memory???");
		mpOptions->OnInit(pOptions);
	}

	mpEventManager = rnew EventManager("Application EventManager", true);
	assert(mpEventManager && "Not enough memory for EventManager");
	

	FLOG(logINFO) << "Loading Text-Strings for language: " << mpOptions->mLanguage.c_str();
	if(FAILED(LoadLanguage(mpOptions->mLanguage.c_str()))) 
		return E_FAIL;

	if(FAILED(CreateMainWindow()))
		return E_FAIL;


	FLOG(logINFO) << "Initializing Render-Interface-DLL.";
	{
		mpRenderer = rnew RIRenderer();
		if(!mpRenderer || FAILED(mpRenderer->CreateRenderInterface(mpOptions->mAPI))) {
			FLOG(logERROR) << "Failed to initialize the RenderInterface.";
			return E_FAIL;
		}
		mpDevice = mpRenderer->GetRenderInterface();
		if(!mpDevice) {
			FLOG(logERROR) << "RIRenderer returns a null-Pointer..??";
			return E_FAIL;
		}
		if(FAILED(mpDevice->VOnInit(mhWnd, mpOptions->mRenderOptions))) {
			FLOG(logERROR) << "RIRenderInterface->VOnInit() failed!";
			return E_FAIL;
		}
		mpDevice->SetClearColor(RICOLOR_ARGB(255, 128, 128, 128));
	}

	// Windows-Stuff
	{
		// Desktop-Resolution
		GetWindowRect(mhWnd, &mDesktopSize);

		mbInitialized = true;
		mbPaused	  = false;
		mbQuitting	  = false;
	}

	VOnInit();

	return S_OK;
}

void Application::OnClose() {
	FLOG(logINFO) << "Releasing Resources";
	SAFE_DELETE(mpRenderer);	// releases the RenderInterface
	SAFE_DELETE(mpEventManager);
	SAFE_DELETE(mpOptions);
	SAFE_DELETE(mpClock);
	CloseLogger();
}

void Application::OnUpdate() {
	if(!mbPaused) {
		mpEventManager->VOnUpdate();

	}
}

void Application::OnRender() {
/*
	FLOG(logDEBUG2) << "S: Application::OnRender()";*/

	VOnUpdate();

/*	FLOG(logDEBUG2) << "F: Application::OnRender()";
	FLOG(logDEBUG2) << "--------------------------";*/
}

// MESSAGES
LRESULT CALLBACK Application::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	assert(gpApp);

	switch(msg) {
		case WM_DESTROY:
			return 0;

		case WM_CLOSE:
			DestroyWindow(hWnd);
				gpApp->VDestroy();
			gpApp->Abort();
			return 0;
			
		case WM_SIZE:
			if(!gpApp->mpDevice)
				break;

			if(wParam == SIZE_MINIMIZED)
				return 0;
			else if(wParam == SIZE_MAXIMIZED) 
				gpApp->mpDevice->VSetWindowed(false);


			return 0;

		case WM_DISPLAYCHANGE:
		{
			U32 colorDepth = (U32)wParam;
			U32 width = (U32)(short) LOWORD(lParam);
			U32 height = (U32)(short) HIWORD(lParam);

			gpApp->OnDisplayChange(colorDepth, width, height);
			return 0;
		}	break;

		case WM_SYSKEYDOWN:
		{
			if (wParam == VK_RETURN) {
				gpApp->OnAltEnter();
				return 0;
			}
		}	break;

		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		{
			// TODO
		} break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void Application::OnAltEnter() {
	gpApp->mpDevice->VSetWindowed(!gpApp->mpDevice->IsWindowed());
}

HRESULT Application::OnDisplayChange(U32 colorDepth, U32 width, U32 height) {
	// Conflict when going to fullscreen; works without Resizing
	return S_OK;
}

int Application::Go() {
	assert(mbInitialized);
	FLOG(logINFO) << "Entering Mainloop.";

	MSG msg = {};

	mpClock->Init();	// Reset clock
	while(!mbQuitting) {
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		mpClock->Update();
		char buffer[1024];
		sprintf(buffer, "FPS: %f", mpClock->GetFPS());
		SetWindowTextA(mhWnd, buffer);

		OnUpdate();

		if(SUCCEEDED(mpDevice->VBeginRendering())) {
			mpDevice->VClear(true, true, true);

			OnRender();

			mpDevice->VEndRendering();
		}
	}

	return msg.wParam;
}

HRESULT Application::CreateMainWindow() {
	FLOG(logINFO) << "Creating MainWindow.";
	WNDCLASSEX wc;
	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.hInstance		= mhInstance;
	wc.lpszClassName	= VGetGameTitle();
	wc.lpfnWndProc		= Application::MsgProc;
	wc.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wc.hIcon			= VGetGameIcon();
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.lpszMenuName		= NULL;
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hIconSm			= VGetGameIcon();

	if(RegisterClassEx(&wc) == 0) {
		FLOG(logERROR) << "Couldn't register window-Class.";
		return E_FAIL;
	}

	mhWnd = CreateWindowExW(0, VGetGameTitle(), VGetGameTitle(), (WS_OVERLAPPEDWINDOW | WS_VISIBLE) & ~(WS_THICKFRAME), 
						   0,
                           0,
						   640, 480, 
						   NULL, NULL, mhInstance, NULL);

	if(mhWnd == NULL) {
		FLOG(logERROR) << "CreateWindowEx failed.";
		return E_FAIL;
	}

	return S_OK;
}

// LOGGING
HRESULT Application::InitLogger() {
	FILELog::ReportingLevel() = logDEBUG4;
	std::string fileName = "Log.html";
	FILE *pFile = fopen(fileName.c_str(), "w");
	if(!pFile) {
		MessageBox(NULL, L"Couldn't open LogFile.", L"error", MB_OK);
		return E_FAIL;
	}
	fprintf(pFile, "<html><head><title>LogFile</title></head>");
	fprintf(pFile, "<body><font face='courier new'>");
	fprintf(pFile, "<table cellspacing='0' cellpadding='0' width='100%%' ");
	fprintf(pFile, "bgcolor='#DFDFE5'>\n<tr>\n<td>\n<font face='arial' ");
	fprintf(pFile, "size='+%i'>\n", 2);
	fprintf(pFile, "LogFile </font>\n</td>\n</tr>\n</table>\n<br>");

#ifdef _DEBUG
	fprintf(pFile, "BUILD: DEBUG<br>");
#else
	fprintf(pFile, "BUILD: RELEASE<br>");
#endif

	fclose(pFile);
	pFile = fopen(fileName.c_str(), "a");
	if(!pFile) {
		MessageBox(NULL, L"Couldn't open LogFile.", L"error", MB_OK);
		return E_FAIL;
	}
	Output2FILE::Stream() = pFile;

	return S_OK;
}

void Application::CloseLogger() {
	if(Output2FILE::Stream()) {
		FILE *pFile = Output2FILE::Stream();
		fprintf(pFile, "<br><br>End.</font></body></html>");
		fclose(Output2FILE::Stream());
		Output2FILE::Stream() = NULL;
	}
}

// Text
HRESULT Application::LoadLanguage( const std::string &language )	{
	std::string file = "Data\\Strings\\";
	file += language;
	file += ".xml";

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError er = doc.LoadFile(file.c_str());
	if(er != tinyxml2::XML_SUCCESS) {
		FLOG(logERROR) << "Failed to load xml File for language: " << language.c_str();
		FLOG(logERROR) << "ErrorCode: " << er; 
		return E_FAIL;
	}

	tinyxml2::XMLElement *pRoot = doc.RootElement();
	if(!pRoot) {
		FLOG(logERROR) << "Strings are missing: " << file.c_str();
		return E_FAIL;
	}

	for(tinyxml2::XMLElement *pElement=pRoot->FirstChildElement(); pElement; pElement=pElement->NextSiblingElement()) {
		assert(pElement);

		const char* pKey = pElement->Attribute("id");
		const char* pText = pElement->Attribute("value");
		if(pKey && pText) {
			wchar_t wKey[64];
			wchar_t wText[1024];
			AnsiToWideCch(wKey, pKey, 64);
			AnsiToWideCch(wText, pText, 1024);
			mText[std::wstring(wKey)] = std::wstring(wText);
		} else 
			FLOG(logWARNING) << "Entry with missing key or value!";
	}

	return S_OK;
}

std::wstring Application::GetString( const std::wstring &textID ) const	{
	auto it = mText.find(textID);
	if(it == mText.end()) {
		FLOG(logERROR) << "Couldn't find text with stringID: " << textID.c_str();
		assert(0 && "String not found!");
		return L"";
	}

	return it->second;
}

void Application::Pause(bool bPause) {
	if(mbPaused == bPause)
		return;

	mbPaused = bPause;
	// Post pause event
}