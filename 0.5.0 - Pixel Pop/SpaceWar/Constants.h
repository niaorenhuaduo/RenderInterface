#pragma once

#include <RI.h>
#include <Vec2.h>
#include <IRenderSystem.h>

// PixelPop
const float MOVEMENT_SPEED		= 800.0f;  // PIXELS per sec
const float BASELINE			= 1080.0f / 2.0f;
const float DEADZONE_X			= -50.0f;
const float HEART_START_Y		= 10.0f;
const U32	MAX_HEALTH			= 7;

const float ANIMATION_HIT_DURATION = 0.2f;


const ri::Vec2 SPAWN_POS = ri::Vec2(2000.0f, BASELINE);
const ri::Vec2 BACKGROUND_POS = ri::Vec2(-390.0f, -260.0f);
const ri::Vec2 BODY_POS = ri::Vec2(207.0f, 130.0f);			//const ri::Vec2 BODY_POS = ri::Vec2(300.0f, 115.0f);

const float HITPOINT_X				= BODY_POS.x + 355.0f;			
const float TIME_TO_REACH_HITPOINT	= (SPAWN_POS.x - HITPOINT_X) / MOVEMENT_SPEED; 

//
const char CLASS_NAME[] = "WinMain";
const char APP_TITLE[] = "SpaceWar";

const char RENDER_API[] = "DirectX9";


// Audio


const char WAVE_BANK[]  = "..//..//Data//PixelPop//Audio//Win//WaveBank.xwb";
const char SOUND_BANK[] = "..//..//Data//PixelPop//Audio//Win//SoundBank.xsb";

//const char WAVE_BANK[]  = "Data//Audio//Win//WaveBank.xwb";
//const char SOUND_BANK[] = "Data//Audio//Win//SoundBank.xsb";

const char BACKGROUND_1AND2[] = "bg";
const char BACKGROUND_3[] = "pointless_test";
const char HIT[]		= "hit";
const char EXPLOSION[]	= "explosion";
const char MENUMUSIC[]  = "menu";
const char FLIP[]		= "flip";
const char WIN[]		= "win";
const char LOST[]		= "lost";


// SONGS DATA
const float SONG1_START = 3.5f;

const float SONG2_START = 3.5f;
const float SONG2_1 = 1.0f;
const float SONG2_2 = 2.0f;
const float SONG2_4 = 4.0f;

const float POINTLESS_1 = 0.44475f;
const float POINTLESS_2 = POINTLESS_1 * 2.0f;
const float POINTLESS_3 = POINTLESS_1 * 3.0f;
const float POINTLESS_4 = POINTLESS_2 * 2.0f;
const float POINTLESS_START = 13.452f;


// MENU
const ri::Vec2 EASY_POS = ri::Vec2(320.0f, 389.0f);
const ri::Vec2 MIDDLE_POS = ri::Vec2(static_cast<float>(ri::VIRTUALXRESOLUTION) / 2.0f, 389.0f);
const ri::Vec2 HARD_POS = ri::Vec2(1324.0f, 389.0f);


// Game
const bool  CAPTURE_MOUSE = false;
const float MAX_FRAMERATE = 99.99f;
const float MIN_FRAMERATE = 10.0f;
const float MAX_ELAPSED_TIME = 1.0f / MIN_FRAMERATE;
const float MIN_ELAPSED_TIME = 1.0f / MAX_FRAMERATE;