#pragma once

#include <RI.h>

const char CLASS_NAME[] = "WinMain";
const char APP_TITLE[] = "SpaceWar";

const char RENDER_API[] = "DirectX9";

// Game
const U32   WINDOW_WIDTH  = 1280;
const U32	WINDOW_HEIGHT = 800;
const bool	WINDOWED	  = true;
const bool	MULTI_SAMPLING= false;
const bool  TEXTURE_FILTER= false;

const char * const TEXTURE_FILES_DIRECTORY = "..\\..\\Data\\Demos\\Textures\\";

const bool  CAPTURE_MOUSE = false;
const float MAX_FRAMERATE = 100.0f;
const float MIN_FRAMERATE = 10.0f;
const float MAX_ELAPSED_TIME = 1.0f / MIN_FRAMERATE;
const float MIN_ELAPSED_TIME = 1.0f / MAX_FRAMERATE;