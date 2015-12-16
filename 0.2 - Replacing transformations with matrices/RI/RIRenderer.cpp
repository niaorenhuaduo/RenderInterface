#include "..\Include\RIRenderer.h"


RIRenderer::RIRenderer() : mpRI(NULL), mhDLL(0) {}
RIRenderer::~RIRenderer() {
	ReleaseRenderInterface();
}

HRESULT RIRenderer::CreateRenderInterface(const std::wstring &sAPI, const std::wstring &options) {
	if(mpRI)
		ReleaseRenderInterface();

	// load desired dll
	if(sAPI == L"DirectX9") {
		std::wstring dllname = L"RID9.dll";
#ifdef _DEBUG
		dllname = L"RID9d.dll";
#endif

		mhDLL = LoadLibrary(dllname.c_str());
		if(!mhDLL) {
			MessageBox(NULL, L"RID9.dll not found!", L"error", MB_OK);
			return E_FAIL;
		}
	} else {
		std::wstring error(L"API '" + sAPI + L"' not supported.");
		MessageBox(NULL, error.c_str(), L"Error", MB_OK);
		return E_FAIL;
	}

	// Create device from dll
	CREATERENDERINTERFACE _CRI = 0;
	_CRI = (CREATERENDERINTERFACE)GetProcAddress(mhDLL, "CreateRenderInterface");
	if(!_CRI) {
		MessageBox(NULL, L"Couldn't find CreateRenderInterface in DLL!", L"error", MB_OK);
		return E_FAIL;
	}

	HRESULT hr = _CRI(mhDLL, options, &mpRI);
	if(FAILED(hr)) {
		MessageBox(NULL, L"'CreateRenderInterface' from lib failed.", L"error", MB_OK);
		mpRI = NULL;
		return E_FAIL;
	}

	return S_OK;
}


void RIRenderer::ReleaseRenderInterface() {
	if(!mhDLL || !mpRI)
		return;

	RELEASERENDERINTERFACE _RRI = 0;
	_RRI = (RELEASERENDERINTERFACE)GetProcAddress(mhDLL, "ReleaseRenderInterface");
	if(!_RRI) {
		MessageBox(NULL, L"Couldn't find ReleaseRenderInterface in DLL!", L"error", MB_OK);
		return;
	}

	HRESULT hr = _RRI(&mpRI);
	if(FAILED(hr)) 
		MessageBox(NULL, L"'ReleaseRenderInterface' from lib failed.", L"error", MB_OK);
	mpRI = NULL;
	mhDLL = NULL;
}