#pragma once

#include <RI.h>

const char CLASS_NAME[] = "WinMain";
const char APP_TITLE[] = "SpaceWar";

const char RENDER_API[] = "DirectX9";

// Game
const bool  CAPTURE_MOUSE = false;
const float MAX_FRAMERATE = 100.0f;
const float MIN_FRAMERATE = 10.0f;
const float MAX_ELAPSED_TIME = 1.0f / MIN_FRAMERATE;
const float MIN_ELAPSED_TIME = 1.0f / MAX_FRAMERATE;