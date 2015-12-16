#pragma once

#include <RI.h>

//
const char CLASS_NAME[] = "WinMain";
const char APP_TITLE[] = "SpaceWar";

const char RENDER_API[] = "DirectX9";


// Audio
const char WAVE_BANK[]  = "..//..//Data//Demos//Audio//AddingSoundManager//Win//WaveBank.xwb";
const char SOUND_BANK[] = "..//..//Data//Demos//Audio//AddingSoundManager//Win//SoundBank.xsb";

const char SPLASH[] = "splash";
const char SONG[] = "song";
const char APPLAUSE[] = "applause";


// Game
const bool  CAPTURE_MOUSE = false;
const float MAX_FRAMERATE = 99.99f;
const float MIN_FRAMERATE = 10.0f;
const float MAX_ELAPSED_TIME = 1.0f / MIN_FRAMERATE;
const float MIN_ELAPSED_TIME = 1.0f / MAX_FRAMERATE;