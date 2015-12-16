#pragma once

/********************************************************************/
// INCLUDES
/********************************************************************/
#include "D9TextureManager.h"


// TODO: Alloc: optimize index copying
// TODO: enable/disable stencil based on renderinterface->mbStencil

// Works fine until now (Adj. Projection by -0.5f) c
// http://msdn.microsoft.com/en-us/library/windows/desktop/bb219690(v=vs.85).aspx

// vertex/index data


namespace ri 
{


#define RIFVF_GUIVERT (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)
struct guiVert_t {
	float x, y, z;
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
	bool			bAlpha;
	PrimitiveType   primitiveType;
};


class D92DRenderer : private UnCopyable {
public:
	D92DRenderer();
	~D92DRenderer();

	void	OnLostDevice();
	bool	OnRestore(LPDIRECT3DDEVICE9 pDevice, const D9TextureManager *pTexMgr);

	bool	BeginFrame();
	guiVert_t* Alloc(U32 numVerts, const index_t *indices, U32 numIndices, PrimitiveType primType, HTexture tex, 
					 bool bAlpha);
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
	static const U32 MAX_VERTS	  = 5000*4;		// = 5000 Quads
	static const U32 MAX_INDICES  = 5000*6;

	static const U32 mStride = sizeof(guiVert_t);
	static const U32 mVBSize = mStride * MAX_VERTS;
	static const U32 mIBSize = sizeof(index_t) * MAX_INDICES;

private:
	void Clear();
	void AdvanceSurfaces(bool bUseIndices);
	void Lock();
	void Unlock();
};



} // namespace ri