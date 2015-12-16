#include "DXGlobal.h"
#include "D9RenderSystem.h"

#include <fstream>

#include "D92DRenderer.h"
#include <Vec2.h>
#include <Vertex.h>
#include <RenderState.h>
#include <ServiceLocator.h>



extern "C" __declspec(dllexport) bool CreateRenderSystem(HINSTANCE hDLL, ri::IRenderSystem **pRI) {
	if(!*pRI) {
		*pRI = rnew ri::D9RenderSystem(hDLL);
		return true;
	}
	return false;
}

extern "C" __declspec(dllexport) bool ReleaseRenderSystem(ri::IRenderSystem **pRI) {
	if(*pRI) {
		SAFE_DELETE(*pRI);
		return true;
	}
	return false;
}

/*----------------------------------------------------------------*/


namespace ri
{


/*-----------------------------------------------------------*/
/* D3D9 implementation		                                 *
/*-----------------------------------------------------------*/
D9RenderSystem::D9RenderSystem(HINSTANCE hDLL) :
	mbStencil(false),
	mpD3D(NULL),
	mpDevice(NULL),
	mpp(),
	mbDeviceLost(true),
	mbRendering(false),
	mp2DRenderer(NULL),
	mCameraWidth(VIRTUALXRESOLUTION),
	mCameraHeight(VIRTUALYRESOLUTION),
	mbYFlipping(true),
	mCameraPosition(0.0f)
{
	assert(hDLL && "RenderInterface impl: No DLL Handle!");
	mbRunning = true;
	mClearColor = Color(64, 90, 64, 255);
	mbTextureFilter = false;
	mbMultiSampling = false;
	mWindowWidth = 640;
	mWindowHeight = 480;
	mbWindowed = true;
	mhWnd = NULL;
	mpTexManager = NULL;
	mhDLL = hDLL;
	mTextureFilesDirectory = "..\\..\\Data\\Textures\\";

	// Viewport
	mViewport.left = mViewport.top = 0.0f;
	mViewport.width  = 1.0f;
	mViewport.height = 1.0f;

	mAdjustedCameraWidth  = mCameraWidth;
	mAdjustedCameraHeight = mCameraHeight;

	// Initialize LogFile
	LogFile::Init("RendererLogfile.txt");

	Log("RenderInterface Logfile:\n\n");
}

D9RenderSystem::~D9RenderSystem() {
	SAFE_DELETE(mp2DRenderer);
	SAFE_DELETE(mpTexManager);

	SAFE_RELEASE(mpDevice);
	SAFE_RELEASE(mpD3D);
	Log("Deleting RenderInterface...\n");
	LogFile::Release();
}
/*----------------------------------------------------------------*/

/*
void D9RenderSystem::LoadOptions(const std::wstring &options) {
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
}*/

bool D9RenderSystem::VOnInit(const RenderSystemSettings &settings) {
	assert(settings.windowHandle && "VOnInit: Window Handle is NULL!");
	mhWnd			= settings.windowHandle;
	mWindowWidth	= settings.windowWidth;
	mWindowHeight	= settings.windowHeight;
	mbWindowed		= settings.bWindowed;

	mTextureFilesDirectory  = settings.mTextureFilesDirectory;
	mbTextureFilter			= settings.bTextureFilter;
	mbMultiSampling			= settings.bMultiSampling;

	if(mWindowWidth < 640 || mWindowWidth > 2560)
		mWindowWidth = 640;
	if(mWindowHeight < 480 || mWindowHeight > 1600)
		mWindowHeight = 480;


	Log("[VOnInit: Initializing...]\n");

	// initialize DirectX 9
	if(!CreateD3DDevice())
		return false;

	Log("Initializing Texture Manager\n");
	mpTexManager = rnew D9TextureManager(mpDevice, mTextureFilesDirectory);
	if(!mpTexManager) {
		LogE("Failed to create Texture Manager!");
		return false;
	}
	ServiceLocator::ProvideTextureManager(mpTexManager);

	Log("Initializing 2DRenderer...\n");
	mp2DRenderer = rnew D92DRenderer();
	D9TextureManager *pTexMgr = dynamic_cast<D9TextureManager*>(mpTexManager);
	if(!mp2DRenderer->OnRestore(mpDevice, pTexMgr))
		return false;


	Log("Initializing default Camera...\n");
	// The default camera projects the virtual window directly on the screen
	_VSetCameraPosition(Vec2(static_cast<float>(VIRTUALXRESOLUTION) / 2.0f, static_cast<float>(VIRTUALYRESOLUTION) / 2.0f));
	_VSetCameraSize(VIRTUALXRESOLUTION, VIRTUALYRESOLUTION);
	_VSetYFlipping(true);

	FloatRect vp;
	vp.left   = vp.top = 0.0f;
	vp.width  = 1.0f;
	vp.height = 1.0f;
	_VSetViewport(vp);

	Log("[VOnInit: ...Initializing finished!]\n");


	mbRunning = true;
	return true;
}


bool D9RenderSystem::OnLostDevice() {
	assert(mpDevice);
	assert(!mbRendering && "Never call OnLostDevice while rendering!");
	/* ---- Release Resources ---- */
	Log("OnLostDevice: Releasing Resources.\n");

	mp2DRenderer->OnLostDevice();

	D9TextureManager *pTexMgr = dynamic_cast<D9TextureManager*>(mpTexManager);
	if(!pTexMgr || !pTexMgr->OnLostDevice())
		return false;

	Log("OnLostDevice: Finished releasing.\n");
	/* --------------------------- */
	UpdatePParams();
	if(FAILED(mpDevice->Reset(&mpp))) {
		LogE("OnLostDevice: Direct3D-Device reset failed!\n");
		LogE("Not all Resources were released or the new Settings are not supported.\n");
		return false;
	};


	Log("OnLostDevice: OK\n");
	return true;
}



void D9RenderSystem::ResetDeviceStates() {
	assert(mpDevice);

	SetRenderStates();
	_VSetViewport(mViewport);

	mpDevice->SetTransform(D3DTS_WORLD, reinterpret_cast<const D3DMATRIX*>(&Matrix2D::IdentityMatrix));
	CalcViewMatrix();
	CalcProjectionMatrix();
}



bool D9RenderSystem::OnRestore() {
	assert(mpDevice);
	assert(!mbRendering && "Never call OnRestore while rendering!");
	/* ---- Restore Resources ---- */
	Log("OnRestore: Start restoring Resources.\n");

	D9TextureManager *pTexMgr = dynamic_cast<D9TextureManager*>(mpTexManager);
	if(!pTexMgr || !pTexMgr->OnRestore(mpDevice)) 
		return false;

	if(!mp2DRenderer->OnRestore(mpDevice, pTexMgr))
		return false;
	Log("OnRestore: Finished restoring.\n");
	/* --------------------------- */

	Log("OnRestore: OK\n");
	return true;
}

bool D9RenderSystem::VOnResize(U32 Width, U32 Height, bool bWindowed) {
	assert(mpDevice);
	assert(!mbRendering && "Never call VOnResize while rendering!");
	Log("[VOnResize: Change Resolution...]\n");
	Log(" VOnResize: Resolutin: %dx%d\n", Width, Height);
	Log(" VOnResize: Windowed %i\n", bWindowed);

	mWindowWidth = Width;
	mWindowHeight = Height;
	mbWindowed = bWindowed;

	if (!mbWindowed) while (ShowCursor(false) >= 0);
	else while (ShowCursor(true) < 0);

	if(!OnLostDevice()) {
		LogE("Failed to reset the Device after resolution change..check Resolution.\n");
		if(!mbWindowed) {
			return VOnResize(Width, Height, true);
		} else {
			MessageBox(NULL, L"Couldn't restore the Device!", NULL, MB_OK);
			mbRunning = false;
			return false;
		}
	}

	ResetDeviceStates();

	if(!OnRestore())
		return false;

	Log("[VOnResize: ...OK]\n");
	return true;
}



static index_t quadIndices[6] = { 3, 0, 2, 2, 0, 1 };
void D9RenderSystem::VDrawPic(const Vertex2D *vertices, const RenderState &renderstate) {
	guiVert_t *pVerts = mp2DRenderer->Alloc(4, &quadIndices[0], 6, renderstate.mTexture, renderstate.mbAlpha);
	assert(pVerts);

	Vec2 pos0 = vertices[0].mPos * renderstate.mTransformation;
	Vec2 pos1 = vertices[1].mPos * renderstate.mTransformation;
	Vec2 pos2 = vertices[2].mPos * renderstate.mTransformation;
	Vec2 pos3 = vertices[3].mPos * renderstate.mTransformation;

	guiVert_t verts[4];
	verts[0].x		= pos0.x;
	verts[0].y		= pos0.y;
 	verts[0].tu		= vertices[0].mTex.x;
	verts[0].tv		= vertices[0].mTex.y;
	verts[0].z		= 0.0f;
	verts[0].col	= vertices[0].mColor;

	verts[1].x		= pos1.x;
	verts[1].y		= pos1.y;
	verts[1].tu		= vertices[1].mTex.x;
	verts[1].tv		= vertices[1].mTex.y;
	verts[1].z		= 0.0f;
	verts[1].col	= vertices[1].mColor;

	verts[2].x		= pos2.x;
	verts[2].y		= pos2.y;
	verts[2].tu		= vertices[2].mTex.x;
	verts[2].tv		= vertices[2].mTex.y;
	verts[2].z		= 0.0f;
	verts[2].col	= vertices[2].mColor;

	verts[3].x		= pos3.x;
	verts[3].y		= pos3.y;
	verts[3].tu		= vertices[3].mTex.x;
	verts[3].tv		= vertices[3].mTex.y;
	verts[3].z		= 0.0f;
	verts[3].col	= vertices[3].mColor;

	memcpy(pVerts, &verts[0], sizeof(guiVert_t)*4);
}

/*
static index_t quadIndices[6] = { 3, 0, 2, 2, 0, 1 };
void D9RenderSystem::VDrawPic(const RIVec4 &topleft, const RIVec4 &topright,
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
}*/
/*

void D9RenderSystem::VDrawPic(float x, float y, float w, float h, 
								 const HTexture tex, float s1, float t1, float s2, float t2, 
								 bool bAlpha, stencilTest_t stencil) {
	VDrawPic(RIVec4(x, y, s1, t1), RIVec4(x+w, y, s2, t1), 
			 RIVec4(x+w, y+h, s2, t2), RIVec4(x, y+h, s1, t2), 
			 tex, bAlpha, stencil);
}*/


bool D9RenderSystem::VBeginRendering() {
	assert(mpDevice);
	assert(!mbRendering && "Already rendering!");

	if(!IsRunning())
		return false;

	if(mbDeviceLost == true) {
		HRESULT hr = mpDevice->TestCooperativeLevel();

		if(hr == D3DERR_DEVICENOTRESET) {
			Log("VBeginRendering: Start restoring the Device!\n");

			if(!OnLostDevice()) 
				return false;

			ResetDeviceStates();

			if(!OnRestore())
				return false;

			Log("Direct3D-Device restored!\n");
			mbDeviceLost = false;
		} else if(hr == S_OK) {
			Log("Direct3D-Device restored!\n");
			mbDeviceLost = false;
		} else {
			Sleep(100);
			return false;
		}
	}

	mpDevice->BeginScene();
	mbRendering = true;

	mp2DRenderer->BeginFrame();													// TODO
	return true;
}



void D9RenderSystem::VClear(bool bRenderT, bool bDepth, bool bStencil) {
	assert(mpDevice);
	assert(mbRendering && "Wrong order of function calls; Don't continue if BeginRendering fails!");

	DWORD dwFlags = 0;
	if(bRenderT)				dwFlags |= D3DCLEAR_TARGET;
	if(bDepth)					dwFlags |= D3DCLEAR_ZBUFFER;
	if(bStencil && mbStencil)	dwFlags |= D3DCLEAR_STENCIL;

	mpDevice->Clear(0, NULL, dwFlags, mClearColor, 1.0f, 0);
}


void D9RenderSystem::VEndRendering() {
	assert(mpDevice);
	assert(mbRendering && "Wrong order of function calls; Don't continue if BeginRendering fails!");


	mp2DRenderer->EndFrame();													

	mpDevice->EndScene();
	mbRendering = false;
	if(FAILED(mpDevice->Present(NULL, NULL, NULL, NULL))) {
		mbDeviceLost = true;
		LogE("Direct3D-Device Lost!\n");
	}
}


void D9RenderSystem::_VSetCameraPosition(const Vec2 &centerPosition) {
	assert(mpDevice);
	mCameraPosition = centerPosition;

	CalcViewMatrix();
}

void D9RenderSystem::_VSetCameraSize(U32 width, U32 height) {
	assert(mpDevice);
	if(width <= 0 || height <= 0) {
		LogE("D9RenderSystem::_VSetCameraSize(): forbidden value for width/height!");
		return;
	}

	mCameraWidth  = width;
	mCameraHeight = height;

	mAdjustedCameraWidth  = mCameraWidth;
	mAdjustedCameraHeight = mCameraHeight;

	CalcProjectionMatrix();
}


void D9RenderSystem::_VSetViewport(const FloatRect &viewPort) {
	assert(mpDevice);


	D3DVIEWPORT9 vp;
	if(viewPort.left < 0.0f || viewPort.top < 0.0f || viewPort.width <= 0.0f || viewPort.height <= 0.0f) {
		LogE("D9RenderSystem::_VSetViewport: Forbidden values for the viewport");
		return;

	} else {
		vp.X		= static_cast<DWORD>(viewPort.left   * GetWindowWidth());
		vp.Y		= static_cast<DWORD>(viewPort.top    * GetWindowHeight());
		vp.Width    = static_cast<DWORD>(viewPort.width  * GetWindowWidth());
		vp.Height   = static_cast<DWORD>(viewPort.height * GetWindowHeight());
		vp.MinZ     = 0.0f;
		vp.MaxZ     = 1.0f;
	}

	mViewport = viewPort;
	mpDevice->SetViewport(&vp);

	CalcProjectionMatrix();
}


void D9RenderSystem::_VSetYFlipping(bool bFlipping) {
	mbYFlipping = bFlipping;

	SetRenderStateCullMode();

	CalcProjectionMatrix();
}



bool D9RenderSystem::CreateD3DDevice() {
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
		return false;
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


	// Create Device
	Log("Create Direct3D-Device...\n");
	HRRELEASE(mpD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, mhWnd, vertProcessing, &mpp, &mpDevice));

	if (!mbWindowed) while (ShowCursor(false) >= 0);
	else while (ShowCursor(true) < 0);

	SetRenderStates();

	Log("Loading Direct3D finished!\n");
	mbDeviceLost = false;
	return true;
}

void D9RenderSystem::SetRenderStates() {
	mpDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	mpDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	SetRenderStateCullMode();

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

void D9RenderSystem::UpdatePParams() {
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



void D9RenderSystem::CalcViewMatrix()
{
	assert(mpDevice);

	mView.Identity();
	mView.Translate(-mCameraPosition);

	mpDevice->SetTransform(D3DTS_VIEW, reinterpret_cast<const D3DMATRIX*>(&mView));
}


void D9RenderSystem::CalcProjectionMatrix()
{
	assert(mpDevice); 

	// Consider the aspect ratio so objects won't be stretched
	float viewportAspect = (mViewport.width * static_cast<float>(GetWindowWidth())) / (mViewport.height * static_cast<float>(GetWindowHeight()));
	float cameraAspect   = static_cast<float>(mCameraWidth) / static_cast<float>(mCameraHeight);

	// Increase the projected space so the aspect ratios match up 
	if(viewportAspect > cameraAspect) {
		mAdjustedCameraWidth  = static_cast<U32>(mCameraHeight * viewportAspect);
		mAdjustedCameraHeight = mCameraHeight;
	} else if(viewportAspect < cameraAspect) {
		mAdjustedCameraHeight = static_cast<U32>(mCameraWidth * (1.0f / viewportAspect));
		mAdjustedCameraWidth  = mCameraWidth;
	}

	float flipY			  =  1.0f;
	if(mbYFlipping) flipY = -1.0f;

	// Calculate the projection
	mProjection.Identity();
	mProjection.Scale(2.0f / static_cast<float>(mAdjustedCameraWidth), flipY * 2.0f / static_cast<float>(mAdjustedCameraHeight));

	mpDevice->SetTransform(D3DTS_PROJECTION, reinterpret_cast<const D3DMATRIX*>(&mProjection));
}

void D9RenderSystem::SetRenderStateCullMode() {
	assert(mpDevice);

	if(mbYFlipping)	mpDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	else			mpDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
}



} // namespace ri