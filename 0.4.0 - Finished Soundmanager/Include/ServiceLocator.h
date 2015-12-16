#pragma once


/********************************************************************/
// INCLUDES
/********************************************************************/
#include "RI.h"
#include <assert.h>


namespace ri {


class ITextureManager;
class IRenderSystem;



////////////////////////////////////////////////////////////
/// \brief Provides global access to some classes, the program gets terminated if a service is requested before it got provided
///
/// This provides the option to wrap interfaces of services
/// in a logger class to log every activity in a separate file
/// OR to provide a NULL service
///
////////////////////////////////////////////////////////////
class RI_EXPORT ServiceLocator {
	friend class IRenderSystem;
public:

	static IRenderSystem& RenderSystem();
	static ITextureManager& TexMgr();

	static void ProvideTextureManager(ITextureManager *pTexMgr);

private:
	static IRenderSystem   *mpRenderSystem;
	static ITextureManager *mpTextureManager;

private:
	static void ProvideRenderSystem(IRenderSystem *pRenderSystem);
};



/********************************************************************/
// IMPLEMENTATION
/********************************************************************/
inline IRenderSystem& ServiceLocator::RenderSystem() {
	assert(mpRenderSystem && "ServiceLocator::RenderSystem(): The IRenderSystem hasn't been initialized yet!");

	return *mpRenderSystem;
}

inline void ServiceLocator::ProvideRenderSystem( IRenderSystem *pRenderSystem ) {
	mpRenderSystem = pRenderSystem;
}



inline ITextureManager& ServiceLocator::TexMgr() { 
	 assert(mpTextureManager && "ServiceLocator::TexMgr(): The ITextureManager hasn't been initialized yet!");

	return *mpTextureManager;
}

inline void ServiceLocator::ProvideTextureManager(ITextureManager *pTexMgr) { 
	assert(pTexMgr && "ServiceLocator::ProvideTextureManager: NULLPOINTER as argument.");

	mpTextureManager = pTexMgr;
}



} // namespace ri