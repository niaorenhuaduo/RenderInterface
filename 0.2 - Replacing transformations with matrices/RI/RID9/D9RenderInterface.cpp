#include "DXGlobal.h"
#include "D9RenderInterface.h"

#include <fstream>

#include "D92DRenderer.h"


/*-----------------------------------------------------------*/
/* DLL stuff implementation                                  *
/*-----------------------------------------------------------*/

/**
 * DLL Entry Point 
 */
BOOL WINAPI DllEntryPoint(HINSTANCE hDll,
                          DWORD     fdwReason,
                          LPVOID    lpvReserved) {
   switch(fdwReason) {
      // called when we attach to the DLL
      case DLL_PROCESS_ATTACH:
         /* dll init/setup stuff */
         break;
      case DLL_PROCESS_DETACH:
         /* dll shutdown/release stuff */
         break;
      default:
         break;
      };
   
   return TRUE;
   } // DllEntryPoint

extern "C" __declspec(dllexport) HRESULT CreateRenderInterface(HINSTANCE hDLL, const std::wstring &options, RIRenderInterface **pRI) {
	if(!*pRI) {
		*pRI = rnew D9RenderInterface(hDLL, options);
		return S_OK;
	}
	return E_FAIL;
}

extern "C" __declspec(dllexport) HRESULT ReleaseRenderInterface(RIRenderInterface **pRI) {
	if(*pRI) {
		SAFE_DELETE(*pRI);
		return S_OK;
	}
	return E_FAIL;
}

/*----------------------------------------------------------------*/




/*-----------------------------------------------------------*/
/* D3D9 implementation		                                 *
/*-----------------------------------------------------------*/
D9RenderInterface::D9RenderInterface(HINSTANCE hDLL, const std::wstring &options) :
	mbStencil(false),
	mpD3D(NULL),
	mpDevice(NULL),
	mpp(),
	mbDeviceLost(true),
	mbRendering(false),
	mp2DRenderer(NULL)
{
	assert(hDLL && "RenderInterface impl: No DLL Handle!");
	mbRunning = true;
	mClearColor = RICOLOR_ARGB(255, 64, 64, 64);
	mbTextureFilter = false;
	mbMultiSampling = false;
	mWindowWidth = 640;
	mWindowHeight = 480;
	mbWindowed = true;
	mhWnd = NULL;
	mpTexManager = NULL;
	mhDLL = hDLL;

	// Initialize LogFile
	LogFile::Init("RendererLogfile.txt");

	Log("RenderInterface Logfile:\n\n");
	LoadOptions(options);
}

D9RenderInterface::~D9RenderInterface() {
	SAFE_DELETE(mp2DRenderer);
	SAFE_DELETE(mpTexManager);

	SAFE_RELEASE(mpDevice);
	SAFE_RELEASE(mpD3D);
	Log("Deleting RenderInterface...\n");
	LogFile::Release();
}
/*----------------------------------------------------------------*/

void D9RenderInterface::LoadOptions(const std::wstring &options) {
	Log("Loading options...\n");
	std::ifstream file(options, std::ios::in);
	if(!file) {
		Log("Couldn't find options file.\n");
		MessageBox(NULL, L"Couldn't open options file.", NULL, MB_OK);
		mWindowWidth = 640;
		mWindowHeight = 480;
		mbStencil = true;
		mbTextureFilter = true;
		mbMultiSampling = true;
		mbWindowed = true;
		mTextureFilesDirectory = "Data\\Textures\\";
		return;
	}

	char buffer[256];
	file >> buffer;
	file >> mWindowWidth;
	file >> buffer;
	file >> mWindowHeight;
	file >> buffer;
	file >> buffer;
	if(strcmp(buffer, "yes") == 0)
		mbWindowed = true;
	else mbWindowed = false;
	mbStencil = true;
	file >> buffer;
	file >> buffer;
	if(strcmp(buffer, "yes") == 0)
		mbTextureFilter = true;
	else mbTextureFilter = false;
	file >> buffer;
	file >> buffer;
	if(strcmp(buffer, "yes") == 0)
		mbMultiSampling = true;
	else mbMultiSampling = false;
	file >> buffer;
	file >> buffer;
	mTextureFilesDirectory = buffer;

	file.close();

	if(mWindowWidth < 640 || mWindowWidth > 2560)
		mWindowWidth = 640;
	if(mWindowHeight < 480 || mWindowHeight > 1600)
		mWindowHeight = 480;

	Log("Loading options finished!\n");
}

HRESULT D9RenderInterface::VOnInit(const HWND hWnd) {
	assert(hWnd && "VOnInit: Window Handle is NULL!");
	mhWnd = hWnd;

	Log("[VOnInit: Initializing...]\n");

	// initialize DirectX 9
	if(FAILED(CreateD3DDevice()))
		return E_FAIL;

	Log("Initializing Texture Manager\n");
	mpTexManager = rnew D9TextureManager(mpDevice, mTextureFilesDirectory);
	if(!mpTexManager) {
		LogE("Failed to create Texture Manager!");
		return E_FAIL;
	}

	Log("Initializing 2DRenderer...\n");
	mp2DRenderer = rnew D92DRenderer();
	D9TextureManager *pTexMgr = dynamic_cast<D9TextureManager*>(mpTexManager);
	if(FAILED(mp2DRenderer->OnRestore(mpDevice, pTexMgr)))
		return E_FAIL;

	Log("[VOnInit: ...Initializing finished!]\n");

	mbRunning = true;
	return S_OK;
}


HRESULT D9RenderInterface::OnLostDevice() {
	assert(mpDevice);
	assert(!mbRendering && "Never call OnLostDevice while rendering!");
	/* ---- Release Resources ---- */
	Log("OnLostDevice: Releasing Resources.\n");

	mp2DRenderer->OnLostDevice();

	D9TextureManager *pTexMgr = dynamic_cast<D9TextureManager*>(mpTexManager);
	if(!pTexMgr || FAILED(pTexMgr->OnLostDevice()))
		return E_FAIL;

	Log("OnLostDevice: Finished releasing.\n");
	/* --------------------------- */
	UpdatePParams();
	if(FAILED(mpDevice->Reset(&mpp))) {
		LogE("OnLostDevice: Direct3D-Device reset failed!\n");
		LogE("Not all Resources were released or the new Settings are not supported.\n");
		return E_FAIL;
	}
	SetRenderStates();

	Log("OnLostDevice: OK\n");
	return S_OK;
}

HRESULT D9RenderInterface::OnRestore() {
	assert(mpDevice);
	assert(!mbRendering && "Never call OnRestore while rendering!");
	/* ---- Restore Resources ---- */
	Log("OnRestore: Start restoring Resources.\n");

	D9TextureManager *pTexMgr = dynamic_cast<D9TextureManager*>(mpTexManager);
	if(!pTexMgr || FAILED(pTexMgr->OnRestore(mpDevice))) 
		return E_FAIL;

	if(FAILED(mp2DRenderer->OnRestore(mpDevice, pTexMgr)))
		return E_FAIL;
	Log("OnRestore: Finished restoring.\n");
	/* --------------------------- */

	Log("OnRestore: OK\n");
	return S_OK;
}

HRESULT D9RenderInterface::VOnResize(U32 Width, U32 Height, bool bWindowed) {
	assert(mpDevice);
	assert(!mbRendering && "Never call VOnResize while rendering!");
	Log("[VOnResize: Change Resolution...]\n");
	Log(" VOnResize: Resolutin: %dx%d\n", Width, Height);
	Log(" VOnResize: Windowed %i\n", bWindowed);

	mWindowWidth = Width;
	mWindowHeight = Height;
	mbWindowed = bWindowed;
	setWindowSize(mWindowWidth, mWindowHeight);

	if (!mbWindowed) while (ShowCursor(false) >= 0);
	else while (ShowCursor(true) < 0);

	if(FAILED(OnLostDevice())) {
		LogE("Failed to reset the Device after resolution change..check Resolution.\n");
		if(!mbWindowed) {
			return VOnResize(Width, Height, true);
		} else {
			MessageBox(NULL, L"Couldn't restore the Device!", NULL, MB_OK);
			mbRunning = false;
			return E_FAIL;
		}
	}
	if(FAILED(OnRestore()))
		return E_FAIL;

	Log("[VOnResize: ...OK]\n");
	return S_OK;
}

static index_t quadIndices[6] = { 3, 0, 2, 2, 0, 1 };
void D9RenderInterface::VDrawPic(const RIVec4 &topleft, const RIVec4 &topright,
								 const RIVec4 &botright, const RIVec4 &botleft,
								 const HTexture tex, 
								 bool bAlpha, stencilTest_t stencil) {
	guiVert_t *pVerts = mp2DRenderer->Alloc(4, &quadIndices[0], 6, tex, bAlpha, stencil);
	assert(pVerts);

	float xscale = (float)mWindowWidth / RIVIRTUALXRESOLUTION;
	float yscale = (float)mWindowHeight / RIVIRTUALYRESOLUTION;
	float scale = min(xscale, yscale);
	float xadj = (mWindowWidth - scale * RIVIRTUALXRESOLUTION)/2.0f - 0.5f;
	float yadj = (mWindowHeight - scale * RIVIRTUALYRESOLUTION)/2.0f - 0.5f;

	guiVert_t verts[4];
	verts[0].x		= topleft.x * scale + xadj;
	verts[0].y		= topleft.y * scale + yadj;
 	verts[0].tu		= topleft.z;
	verts[0].tv		= topleft.w;
	verts[0].rhw	= 1.0f;
	verts[0].z		= 0.0f;
	verts[0].col	= RIWhite;

	verts[1].x		= topright.x * scale + xadj;
	verts[1].y		= topright.y * scale + yadj;
	verts[1].tu		= topright.z;
	verts[1].tv		= topright.w;
	verts[1].rhw	= 1.0f;
	verts[1].z		= 0.0f;
	verts[1].col	= RIWhite;

	verts[2].x		= botright.x * scale + xadj;
	verts[2].y		= botright.y * scale + yadj;
	verts[2].tu		= botright.z;
	verts[2].tv		= botright.w;
	verts[2].rhw	= 1.0f;
	verts[2].z		= 0.0f;
	verts[2].col	= RIWhite;

	verts[3].x		= botleft.x * scale + xadj;
	verts[3].y		= botleft.y * scale + yadj;
	verts[3].tu		= botleft.z;
	verts[3].tv		= botleft.w;
	verts[3].rhw	= 1.0f;
	verts[3].z		= 0.0f;
	verts[3].col	= RIWhite;

	memcpy(pVerts, &verts[0], sizeof(guiVert_t)*4);
}

void D9RenderInterface::VDrawPic(float x, float y, float w, float h, 
								 const HTexture tex, float s1, float t1, float s2, float t2, 
								 bool bAlpha, stencilTest_t stencil) {
	VDrawPic(RIVec4(x, y, s1, t1), RIVec4(x+w, y, s2, t1), 
			 RIVec4(x+w, y+h, s2, t2), RIVec4(x, y+h, s1, t2), 
			 tex, bAlpha, stencil);
}


HRESULT D9RenderInterface::VBeginRendering() {
	assert(mpDevice);
	assert(!mbRendering && "Already rendering!");

	if(!IsRunning())
		return E_FAIL;

	if(mbDeviceLost == true) {
		HRESULT hr = mpDevice->TestCooperativeLevel();

		if(hr == D3DERR_DEVICENOTRESET) {
			Log("VBeginRendering: Start restoring the Device!\n");

			if(FAILED(OnLostDevice())) 
				return E_FAIL;
			if(FAILED(OnRestore()))
				return E_FAIL;

			Log("Direct3D-Device restored!\n");
			mbDeviceLost = false;
		} else if(hr == S_OK) {
			Log("Direct3D-Device restored!\n");
			mbDeviceLost = false;
		} else {
			Sleep(100);
			return E_FAIL;
		}
	}

	mpDevice->BeginScene();
	mbRendering = true;

	mp2DRenderer->BeginFrame();													// TODO
	return S_OK;
}


void D9RenderInterface::VClear(bool bRenderT, bool bDepth, bool bStencil) {
	assert(mpDevice);
	assert(mbRendering && "Wrong order of function calls; Don't continue if BeginRendering fails!");

	DWORD dwFlags = 0;
	if(bRenderT)				dwFlags |= D3DCLEAR_TARGET;
	if(bDepth)					dwFlags |= D3DCLEAR_ZBUFFER;
	if(bStencil && mbStencil)	dwFlags |= D3DCLEAR_STENCIL;

	mpDevice->Clear(0, NULL, dwFlags, mClearColor, 1.0f, 0);
}


void D9RenderInterface::VEndRendering() {
	assert(mpDevice);
	assert(mbRendering && "Wrong order of function calls; Don't continue if BeginRendering fails!");

	mp2DRenderer->EndFrame();													// TODO

	mpDevice->EndScene();
	mbRendering = false;
	if(FAILED(mpDevice->Present(NULL, NULL, NULL, NULL))) {
		mbDeviceLost = true;
		LogE("Direct3D-Device Lost!\n");
	}
}


HRESULT D9RenderInterface::CreateD3DDevice() {
	SAFE_RELEASE(mpDevice);
	SAFE_RELEASE(mpD3D);

	Log("Loading Direct3D...\n");
	mpD3D = Direct3DCreate9(D3D_SDK_VERSION);
	assert(mpD3D && "Direct3DCreate9 failed!");

	// check for hardware-vertexprocessing
	D3DCAPS9 caps;
	DWORD vertProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	HRRELEASE(mpD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps));
    if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
        vertProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
    else
        vertProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	// check for stencil
	if( FAILED( mpD3D->CheckDeviceFormat( D3DADAPTER_DEFAULT,
                                       D3DDEVTYPE_HAL,  
                                       D3DFMT_X8R8G8B8,							// windowed?
                                       D3DUSAGE_DEPTHSTENCIL, 
                                       D3DRTYPE_SURFACE,
                                       D3DFMT_D24S8 ) ) ) {
		LogE("CreateD3DDevice: 8bit Stencil not supported.");
		assert(0 && "Stencilbuffer not supported!");
		return E_FAIL;
	}

	// Fill Presentparams
	ZeroMemory(&mpp, sizeof(D3DPRESENT_PARAMETERS));
	if(mbWindowed) {
		mpp.Windowed		 = TRUE;
		mpp.BackBufferFormat = D3DFMT_UNKNOWN;  // Windows display-settings
		mpp.BackBufferWidth  = mWindowWidth;
		mpp.BackBufferHeight = mWindowHeight;
	} else {
		mpp.Windowed		 = FALSE;
		mpp.BackBufferWidth  = mWindowWidth;
		mpp.BackBufferHeight = mWindowHeight;
		mpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	}
	mpp.BackBufferCount			= 1;
	mpp.MultiSampleQuality		= 0;
	mpp.MultiSampleType			= D3DMULTISAMPLE_NONE;
	mpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;
	mpp.hDeviceWindow			= mhWnd;
	mpp.EnableAutoDepthStencil	= TRUE;
	if(mbStencil)
		mpp.AutoDepthStencilFormat	= D3DFMT_D24S8;
	else 
		mpp.AutoDepthStencilFormat = D3DFMT_D16;
	
	UpdatePParams();	// Set multisampling

	setWindowSize(mWindowWidth, mWindowHeight);
	// Create Device
	Log("Create Direct3D-Device...\n");
	HRRELEASE(mpD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, mhWnd, vertProcessing, &mpp, &mpDevice));

	if (!mbWindowed) while (ShowCursor(false) >= 0);
	else while (ShowCursor(true) < 0);

	SetRenderStates();

	Log("Loading Direct3D finished!\n");
	mbDeviceLost = false;
	return S_OK;
}

void D9RenderInterface::SetRenderStates() {
	mpDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	mpDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	mpDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	if(mbTextureFilter) {
		mpDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
		mpDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		mpDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	} else {
		mpDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
		mpDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);
		mpDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
	}
}

void D9RenderInterface::UpdatePParams() {
	mpp.Windowed = mbWindowed;
	mpp.BackBufferWidth = mWindowWidth;
	mpp.BackBufferHeight = mWindowHeight;

	// multisampling
	DWORD multisamplequality = 0;
	if( mbMultiSampling && SUCCEEDED(mpD3D->CheckDeviceMultiSampleType( D3DADAPTER_DEFAULT, 
								D3DDEVTYPE_HAL, mpp.BackBufferFormat, 
                                mbWindowed, D3DMULTISAMPLE_2_SAMPLES, &multisamplequality ) ) &&
         SUCCEEDED(mpD3D->CheckDeviceMultiSampleType( D3DADAPTER_DEFAULT, 
                                D3DDEVTYPE_HAL, mpp.AutoDepthStencilFormat, 
                                FALSE, D3DMULTISAMPLE_2_SAMPLES, &multisamplequality ) ) )  {
		mpp.MultiSampleType = D3DMULTISAMPLE_2_SAMPLES;
		mpp.MultiSampleQuality = multisamplequality-1;
	}
}
