#include "..\Include\IRenderSystem.h"
#include "..\Include\DynLibrary.h"
#include "..\Include\ServiceLocator.h"



namespace 
{
	ri::DynLibrary *pRenderSystemDLL = NULL;

}



namespace ri 
{

bool IRenderSystem::CreateRenderSystem( const std::string &api )
{
	// Release the already loaded RenderSystem
	if(pRenderSystemDLL) {
		ReleaseRenderSystem();
	}

	if(api == "DirectX9") {
		std::string dllname = "RSD9.dll";
#ifdef _DEBUG
		dllname = "RSD9d.dll";
#endif

		// Load the library
		pRenderSystemDLL = rnew DynLibrary(dllname);
		if(!pRenderSystemDLL->Load()) return false;

	} else {
		std::string error("API '" + api + "' not supported.");
		MessageBox(NULL, error.c_str(), "Error", MB_OK);
		return false;
	}

	
	// Create device from dll
	CREATERENDERSYSTEM _CRS = 0;
	_CRS = (CREATERENDERSYSTEM)pRenderSystemDLL->GetSymbol("CreateRenderSystem");
	if(!_CRS) {
		MessageBox(NULL, "Couldn't find CreateRenderSystem in DLL!", "error", MB_OK);
		return false;
	}

	IRenderSystem *pRenderSystem = NULL;
	bool result = _CRS(pRenderSystemDLL->GetModule(), &pRenderSystem);

	if(result == false || pRenderSystem == NULL) {
		MessageBox(NULL, "'CreateRenderSystem' from lib failed.", "error", MB_OK);
		return false;
	}

	ServiceLocator::ProvideRenderSystem(pRenderSystem);
	return true;
}

void IRenderSystem::ReleaseRenderSystem() 
{
	if(!pRenderSystemDLL) return;


	RELEASERENDERSYSTEM _RRS = 0;
	_RRS = (RELEASERENDERSYSTEM)pRenderSystemDLL->GetSymbol("ReleaseRenderSystem");
	if(!_RRS) {
		MessageBox(NULL, "Couldn't find ReleaseRenderSystem in DLL!", "error", MB_OK);
		ServiceLocator::ProvideRenderSystem(NULL);
		SAFE_DELETE(pRenderSystemDLL);
		return;
	}

	IRenderSystem *pRenderSystem = &ServiceLocator::RenderSystem();
	bool result = _RRS(&pRenderSystem);

	if(!result) {
		MessageBox(NULL, "'ReleaseRenderSystem' from lib failed.", "error", MB_OK);
		ServiceLocator::ProvideRenderSystem(NULL);
		SAFE_DELETE(pRenderSystemDLL);
		return;
	}

	ServiceLocator::ProvideRenderSystem(NULL);
	SAFE_DELETE(pRenderSystemDLL);
}


} // namespace ri