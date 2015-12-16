#pragma once

#include <string>
#include <windows.h>

bool IsOnlyInstance(const LPCTSTR title);		// Checks for windows with title as the window class name

class GameOptions {
public:
	std::wstring mAPI;
	std::wstring mRenderOptions;
	std::string  mLanguage;

public:
	GameOptions();
	~GameOptions() {}
	void OnInit(const char* const file);
};

