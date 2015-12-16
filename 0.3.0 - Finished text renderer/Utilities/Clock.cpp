#include "Clock.h"

HRESULT Clock::Init() {	
	LARGE_INTEGER Counter;
	if(!QueryPerformanceFrequency(&mCyclesPerSecond) || !QueryPerformanceCounter(&Counter)) {
		assert(0 && "QueryPerformanceFrequency/Counter not supported.");
		FLOG(logERROR) << "QueryPerformanceFrequency/Counter not supported.";
		return E_FAIL;
	}

	mTimeCycles = Counter.QuadPart;
	return S_OK;
} 

Clock::~Clock() {
	if(mTimerList.size()) {
		FLOG(logWARNING) << "Clock: Not all timers were released!";
		assert(0 && "Clock: Not all timers were released.");

		std::list<Timer*>::iterator it = mTimerList.begin();
		while(it != mTimerList.end()) {
			if((*it) != NULL) {
				delete (*it);
				(*it) = NULL;
			}
		}
	}
}

Timer* Clock::CreateTimer(double startTimerSeconds) {
	Timer *pNew = rnew Timer(startTimerSeconds);
	assert(pNew && "Clock::CreateTimer: Out of memory");

	mTimerList.push_back(pNew);

	return pNew;
}

void Clock::ReleaseTimer(Timer *pTimer) {
	mTimerList.remove(pTimer);
	SAFE_DELETE(pTimer);
}