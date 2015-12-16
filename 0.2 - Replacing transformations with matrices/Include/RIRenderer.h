#pragma once

#include "RIRenderInterface.h"

// Creates the desired RenderInterface API implementation
class RIRenderer : private UnCopyable {
public:
	RIRenderer();
	~RIRenderer();

	HRESULT			   CreateRenderInterface(const std::wstring &sAPI, const std::wstring &options);
	void			   ReleaseRenderInterface();

	RIRenderInterface*   GetRenderInterface() const { return mpRI; }
	HMODULE				 GetModule() const { return mhDLL; }
private:
	RIRenderInterface *mpRI;
	HMODULE			   mhDLL;
};