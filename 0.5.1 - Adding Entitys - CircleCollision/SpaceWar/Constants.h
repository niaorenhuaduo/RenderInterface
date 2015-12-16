#pragma once

#include <RI.h>
#include <Vec2.h>
#include <IRenderSystem.h>


//
const char CLASS_NAME[] = "WinMain";
const char APP_TITLE[] = "test";

const char RENDER_API[] = "DirectX9";


// Audio
const char WAVE_BANK[]  = "";
const char SOUND_BANK[] = "";


// Entity const data
const ri::IntRect CIRCLE_RECT   = ri::IntRect(0, 0, 200, 200);
const ri::IntRect QUAD_RECT		= ri::IntRect(104, 2, 100, 100);
const ri::IntRect RECT_RECT		= ri::IntRect(2, 104, 150, 75);
const ri::IntRect SHIP_RECT		= ri::IntRect(2, 181, 120, 60);

const RECT  COLLISION_RECTANGLE = {-75, -37, 75, 37};
const RECT  COLLISION_BOX = {-50, -50, 50, 50};
const RECT  COLLISION_SHIP = { -60, -30, 60, 30 };
const float CIRCLE_RADIUS = 100.0f;


// Game
const bool  CAPTURE_MOUSE = false;
const float MAX_FRAMERATE = 99.99f;
const float MIN_FRAMERATE = 10.0f;
const float MAX_ELAPSED_TIME = 1.0f / MIN_FRAMERATE;
const float MIN_ELAPSED_TIME = 1.0f / MAX_FRAMERATE;