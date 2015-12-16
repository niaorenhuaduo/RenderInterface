#pragma once


#include "D9TextureManager.h"

// TODO: Alloc: optimize index copying
// TODO: enable/disable stencil based on renderinterface->mbStencil

// Works fine until now (Adj. Projection by -0.5f) c
// http://msdn.microsoft.com/en-us/library/windows/desktop/bb219690(v=vs.85).aspx

// vertex/index data
#define RIFVF_GUIVERT (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
struct guiVert_t {
	float x, y, z;
	float rhw;
	DWORD col;
	float tu, tv;
};


typedef U16 index_t;	// if changed: change D3DFMT_INDEX in Constructor

// surface
struct guiSurface_t {
	HTexture		tex;
	U32				firstIndex;
	U32				numVertices;
	U32				numIndices;
	stencilTest_t	stencil;
	bool			bAlpha;

	guiSurface_t& operator=(const guiSurface_t &rhs) { tex = rhs.tex; bAlpha = rhs.bAlpha;
			stencil = rhs.stencil; firstIndex = rhs.firstIndex; numIndices = rhs.numIndices; return *this; }
};


class D92DRenderer : private UnCopyable {
public:
	D92DRenderer();
	~D92DRenderer();

	void	OnLostDevice();
	HRESULT OnRestore(LPDIRECT3DDEVICE9 pDevice, const D9TextureManager *pTexMgr);

	HRESULT BeginFrame();
	guiVert_t* Alloc(U32 numVerts, const index_t *indices, U32 numIndices, HTexture tex, 
					 bool bAlpha, stencilTest_t stencil);
	void	EndFrame();
private:
	LPDIRECT3DDEVICE9			mpDevice;
	const D9TextureManager	   *mpTexMgr;

	LPDIRECT3DVERTEXBUFFER9 mpVB;	// write only
	LPDIRECT3DINDEXBUFFER9	mpIB;	// write only

	U32 mNumVerts;
	U32 mNumIndices;

	bool		 mbLocked;
	guiVert_t	*mpLockedVerts;
	index_t		*mpLockedIndices;

	// surfaces
	std::vector<guiSurface_t>  mSurfaces;
	guiSurface_t			  *mpActSurf;

	// constants
	static const U32 MAX_VERTS	  = 200*4;		// = 100 Quads
	static const U32 MAX_INDICES  = 200*6;

	static const U32 mStride = sizeof(guiVert_t);
	static const U32 mVBSize = mStride * MAX_VERTS;
	static const U32 mIBSize = sizeof(index_t) * MAX_INDICES;

private:
	void Clear();
	void AdvanceSurfaces();
	void Lock();
	void Unlock();
};