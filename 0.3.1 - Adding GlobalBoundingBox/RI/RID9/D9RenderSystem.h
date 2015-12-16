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
	bool OnInit(const RenderSystemSettings &settings);
	bool OnResize(U32 mWidth, U32 mHeight, bool bWindowed);

	bool BeginRendering();
	void Clear(bool bRenderT, bool bDepth, bool bStencil);

	void DrawPic(const Vertex2D *vertices, const RenderState &renderstate);

	void EndRendering();


public:
	// Drawing
	virtual void _Draw(const Vertex2D *vertices, U32 numVertices, PrimitiveType primitiveType, const RenderState &renderstate);

	// Set Camera Position/Size
	void _SetCameraPosition(const Vec2 &centerPosition);		
	void _SetCameraSize(U32 width, U32 height);
	void _SetViewport(const FloatRect &viewPort);

	void _CalcAndSetViewMatrix();
	void _CalcAndSetProjMatrix();


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


	// Device reset required?
	bool					mbRendering;
	bool					mbDeviceLost;

	std::string				mTextureFilesDirectory;
	std::string				mFontFilesDirectory;

private:
	void LoadOptions(const std::wstring &file);
	bool CreateD3DDevice();
	void SetRenderStates();

	bool OnLostDevice();
	bool OnRestore();
	void UpdatePParams();
	void ResetDeviceStates();


	////////////////////////////////////////////////////////////
	/// \brief Flush the 2D Renderer
	///
	////////////////////////////////////////////////////////////
	void Flush();
};



} // namespace ri