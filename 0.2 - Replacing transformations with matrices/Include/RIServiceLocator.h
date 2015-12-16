#pragma once

#include <assert.h>

// ================================================
// This class provides global access to some classes
// This enables the option to wrap the interface of a service
// in a logger class to log every activity in a separate file
// ==
// The implementation DLL has to provide the services
// ================================================
class RITextureManager;

class RIServiceLocator {
public:
	static RITextureManager& GetTexMgr() { 
		assert(mpTextureManager && "RILocator::GetTxMgr(): The RITextureManager hasn't been initialized!");

		return *mpTextureManager;
	}

	static void ProvideTextureManager(RITextureManager *pTexMgr) { 
		assert(pTexMgr && "RILocator::ProvideTextureManager: NULLPOINTER as argument.");

		mpTextureManager = pTexMgr;
	}
private:
	static RITextureManager *mpTextureManager;
};