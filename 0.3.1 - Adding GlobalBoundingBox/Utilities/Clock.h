#pragma once

#include "..\Global.h"
#include "..\Debugging\Logger.h"
#include <assert.h>

// TODO: Optimize Clock::update

const double gUpperLimitTime = 1.0/5.0;		// minimum 5 frames per second
const double gOneFrameTime	 = 1.0/30.0;

class Timer;

// Clock: Used by the application layer, no direct access to the time
class Clock {
	friend class Application;
public:
	Clock() : mTimeCycles(0), mElapsedTime(0.0), mTimerList() {};
	~Clock();	

	Timer* CreateTimer(double startTimerSeconds = 0.0);
	void   ReleaseTimer(Timer *pTimer);

	double  GetFPS() const				{ return 1.0 / mElapsedTime; }
	double	GetRealElapsedTime() const	{ return mElapsedTime; }
private:
	LARGE_INTEGER mCyclesPerSecond;		// const

	U64			  mTimeCycles;			// Time in cycles
	double		  mElapsedTime;			// Elapsed time in seconds

	std::list<Timer*> mTimerList;

private:
	HRESULT Init();
	void	Update();	

	U64	   secondsToCycles(float fTime) { return static_cast<U64>(fTime * mCyclesPerSecond.QuadPart);	}
	double cyclesToSeconds(U64 cycles) { return static_cast<double>(cycles) / static_cast<double>(mCyclesPerSecond.QuadPart); }
};


// Timer: Create and append to the Clock; Gets updated automatically
class Timer : private UnCopyable {
	friend class Clock;
public:
	void SingleStep();		// Continue for one frame when paused

	void Pause()   { mbPaused = true; }
	void UnPause() { mbPaused = false; }

	void   SetScaling(double fScal) { mScaling = fScal; }
	double GetScaling() const	   { return mScaling; }

	U64		GetTime() const		   { return mTime; }
	double  GetElapsedTime() const { return mElapsedTime; }
private:
	Timer(double startTimeSeconds = 0.0) : mTime(static_cast<U64>(startTimeSeconds * 1000.0)), mElapsedTime(0.0), mbPaused(false), mScaling(1.0) {}
	~Timer() {}

	U64		mTime;				// in milliseconds
	double	mElapsedTime;		// in seconds

	bool   mbPaused;
	double mScaling;

	void Update(double fElapsedTime);
};

// Clock
inline void Clock::Update() {
	assert(mTimeCycles && mCyclesPerSecond.QuadPart && "Clock not initialized!");
	LARGE_INTEGER actCycles;
	QueryPerformanceCounter(&actCycles);

	U64	   uDelta = (actCycles.QuadPart - mTimeCycles);
	double fDelta = cyclesToSeconds(uDelta);

	if(fDelta > gUpperLimitTime) fDelta = gOneFrameTime;

	mElapsedTime = fDelta;
	mTimeCycles  = actCycles.QuadPart;

	// update timers
	for(Timer *it : mTimerList) {
		if(it == NULL) {
			FLOG(logWARNING) << "NULL-Timer in Timer-List found... this shouldn't happen.";
			continue;
		}

		it->Update(mElapsedTime);
	}
}


// Timer
inline void Timer::Update(double fElapsedTime) {
	if(mbPaused) {
		mElapsedTime = 0.0;
		return;
	}

	mElapsedTime  = mScaling * fElapsedTime;
	mTime		 += static_cast<U64>(mElapsedTime * 1000.0);
}

inline void Timer::SingleStep() {
	if(mbPaused) {
		mElapsedTime = gOneFrameTime;
		mTime		+= static_cast<U64>(mElapsedTime * 1000.0);
	}
}