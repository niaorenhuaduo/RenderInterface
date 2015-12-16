#pragma once

#include <Windows.h>

// TODO: OnD3DCreate: Check for stencil, windowed backbuffer format
// TODO: VDrawPic: Opimize the resizing
// TODO: DrawPic helper(rotation/scaling/rect usw.)

// DLL-Stuff
BOOL WINAPI DLLEntryPoint(HINSTANCE hDll,
                          DWORD     fdwReason,
                          LPVOID    lpvRserved); 

#include "..\RIRenderInterface.h"
#include "D9TextureManager.h"

extern "C" __declspec(dllexport) HRESULT CreateRenderInterface(HINSTANCE hDLL, const std::wstring &options, RIRenderInterface **pRI);
extern "C" __declspec(dllexport) HRESULT ReleaseRenderInterface(RIRenderInterface **pRI);

class D92DRenderer;

// Direct9 RenderInterface implementation
class D9RenderInterface : public RIRenderInterface {
public:
	D9RenderInterface(HINSTANCE hDLL, const std::wstring &options);
	~D9RenderInterface();

	// Init/Destroy
	HRESULT VOnInit(const HWND hWnd);
	HRESULT VOnResize(U32 mWidth, U32 mHeight, bool bWindowed);

	HRESULT VBeginRendering();
	void	VClear(bool bRenderT, bool bDepth, bool bStencil);
	void	VDrawPic(const RIVec4 &topleft, const RIVec4 &topright,
					 const RIVec4 &botright, const RIVec4 &botleft,
					 const HTexture tex, guiOrder_t zord,
					 bool bAlpha, stencilTest_t stencil);
	void	VDrawPic(float x, float y, float w, float h, 
					 const HTexture tex, float s1, float t1, float s2, float t2, 
					 guiOrder_t zord, bool bAlpha, stencilTest_t stencil);
	void	VEndRendering();

	// Set/Get
	RITextureManager* GetTexMgr()	  { return mpTexManager; }
private:
	D9TextureManager	   *mpTexManager;
	D92DRenderer		   *mp2DRenderer;

	LPDIRECT3D9				mpD3D;
	LPDIRECT3DDEVICE9		mpDevice;
	D3DPRESENT_PARAMETERS	mpp;
	bool					mbStencil;

	bool					mbRendering;
	bool					mbDeviceLost;

	std::string				mTextureFilesDirectory;

private:
	void	LoadOptions(const std::wstring &file);
	HRESULT CreateD3DDevice();
	void	SetRenderStates();

	HRESULT OnLostDevice();
	HRESULT OnRestore();
	void	UpdatePParams();
};
