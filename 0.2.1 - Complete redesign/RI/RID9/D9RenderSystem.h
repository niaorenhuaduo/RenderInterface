#pragma once


// TODO: OnD3DCreate: Check for stencil, windowed backbuffer format
// TODO: VDrawPic: Opimize the resizing
// TODO: DrawPic helper(rotation/scaling/rect usw.)



/********************************************************************/
// INCLUDES
/********************************************************************/
#include <IRenderSystem.h>
#include <Matrix2D.h>
#include "D9TextureManager.h"

extern "C" __declspec(dllexport) bool CreateRenderSystem(HINSTANCE hDLL, ri::IRenderSystem **pRI);
extern "C" __declspec(dllexport) bool ReleaseRenderSystem(ri::IRenderSystem **pRI);


namespace ri
{


class D92DRenderer;
class RenderState;

// Direct9 RenderInterface implementation
class D9RenderSystem : public IRenderSystem {
public:
	D9RenderSystem(HINSTANCE hDLL);
	~D9RenderSystem();

	// Init/Destroy
	bool VOnInit(const RenderSystemSettings &settings);
	bool VOnResize(U32 mWidth, U32 mHeight, bool bWindowed);

	bool VBeginRendering();
	void VClear(bool bRenderT, bool bDepth, bool bStencil);

	void VDrawPic(const Vertex2D *vertices, const RenderState &renderstate);

	void VEndRendering();


public:
	// Set Camera Position/Size
	void _VSetCameraPosition(const Vec2 &centerPosition);		
	void _VSetCameraSize(U32 width, U32 height);
	void _VSetViewport(const FloatRect &viewPort);
	void _VSetYFlipping(bool bFlipping);


private:
	D92DRenderer		   *mp2DRenderer;

	// DirectX stuff
	LPDIRECT3D9				mpD3D;
	LPDIRECT3DDEVICE9		mpDevice;
	D3DPRESENT_PARAMETERS	mpp;
	bool					mbStencil;

	// View and Projection
	Vec2					mCameraPosition;
	U32						mCameraWidth, mCameraHeight;
	U32						mAdjustedCameraWidth, mAdjustedCameraHeight;	// A border is created if the aspect ratio of the viewport/camera don't match
	FloatRect				mViewport;
	Matrix2D				mView;
	Matrix2D				mProjection;
	bool					mbYFlipping;


	// Device reset required?
	bool					mbRendering;
	bool					mbDeviceLost;

	std::string				mTextureFilesDirectory;

private:
	void LoadOptions(const std::wstring &file);
	bool CreateD3DDevice();
	void SetRenderStates();
	void SetRenderStateCullMode();

	bool OnLostDevice();
	bool OnRestore();
	void UpdatePParams();
	void ResetDeviceStates();

	void CalcViewMatrix();
	void CalcProjectionMatrix();
};



} // namespace ri