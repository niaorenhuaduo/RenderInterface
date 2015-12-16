#pragma once

#include <RI.h>

const WCHAR CLASS_NAME[] = L"WinMain";
const WCHAR APP_TITLE[] = L"SpaceWar";

const WCHAR RENDER_API[] = L"DirectX9";
const WCHAR OPTIONS_FILE[] = L"options.dat";

// Game
const bool  CAPTURE_MOUSE = false;
const float MAX_FRAMERATE = 100.0f;
const float MIN_FRAMERATE = 10.0f;
const float MAX_ELAPSED_TIME = 1.0f / MIN_FRAMERATE;
const float MIN_ELAPSED_TIME = 1.0f / MAX_FRAMERATE;