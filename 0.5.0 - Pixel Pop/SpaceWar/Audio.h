#pragma once

#define WIN32_LEAN_AND_MEAN
#include <xact3.h>
#include "Constants.h"


/********************************************************************/
// TODO: Error messaging
/********************************************************************/

class Audio {
public:

	Audio();
	virtual ~Audio();

	bool OnInitialize(const char waveBankFile[], const char soundBankFile[]);
	void OnUpdate();

	void PlayCue(const char cue[]);
	void StopCue(const char cue[]);


private:

	IXACT3Engine	*mpEngine;
	IXACT3WaveBank	*mpWaveBank;
	IXACT3SoundBank *mpSoundBank;
	XACTINDEX		 mCueIndex; 
	void *mpMapWaveBank;
	void *mpSoundBankData;
	bool mbCoInitialized;
};