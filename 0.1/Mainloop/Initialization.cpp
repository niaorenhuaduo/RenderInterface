#include "Initialization.h"

#include "..\Debugging\Logger.h"
#include <assert.h>
#include <fstream>
#include "..\Utilities\String.h"

bool IsOnlyInstance(const LPCTSTR title) {
	HANDLE handle = CreateMutex(NULL, TRUE, title);

	if(GetLastError() != ERROR_SUCCESS) {
		HWND hWnd = FindWindow(title, NULL);
		if(hWnd) {
			ShowWindow(hWnd, SW_SHOWNORMAL);
			SetFocus(hWnd);
			SetForegroundWindow(hWnd);
			SetActiveWindow(hWnd);
			return false;
		}
	}

	return true;
}

GameOptions::GameOptions() :
	mRenderOptions(L"Data\\RenderOptions.txt"),
	mAPI(L"Direct9"),
	mLanguage("German")
{}

void GameOptions::OnInit(const char* const file) {
	assert(file);

	std::ifstream in(file, std::ios::in);
	if(!in) {
		FLOG(logWARNING) << "Couldn't find the GameOptions-File! Loading default settings.";
		return;
	}

	char buffer[256];
	in >> buffer; // RenderOptions
	mRenderOptions.assign(s2ws(buffer));
	in >> buffer;
	in >> buffer; // API
	mAPI.assign(s2ws(buffer));
	in >> buffer;
	in >> buffer;
	mLanguage.assign(buffer);

	FLOG(logINFO) << "GameOptions configuration: ";
	FLOG(logINFO) << "*		API: " << ws2s(mAPI);
	FLOG(logINFO) << "*		Language: " << mLanguage.c_str();
}