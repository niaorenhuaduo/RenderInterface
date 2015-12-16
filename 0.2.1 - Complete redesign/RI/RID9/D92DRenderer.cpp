#include "DXGlobal.h"
#include "D92DRenderer.h"

namespace ri 
{

D92DRenderer::D92DRenderer() :
	mpVB(NULL),
	mpIB(NULL),
	mpDevice(NULL),
	mNumVerts(0),
	mNumIndices(0),
	mpLockedVerts(NULL),
	mpLockedIndices(NULL),
	mbLocked(false),
	mpActSurf(NULL)
{ assert(mVBSize > 0 && mIBSize > 0); }

D92DRenderer::~D92DRenderer() {
	if(mbLocked) {
		LogE("D92DRenderer::~D92DRenderer: Releasing locked buffers!\n");
		Unlock();
	}
	Log("D92DRenderer::~D92DRenderer: Destroying.\n");
	SAFE_RELEASE(mpVB);
	SAFE_RELEASE(mpIB);
}

void D92DRenderer::Clear() {
	mSurfaces.clear();
	AdvanceSurfaces();
}

void D92DRenderer::AdvanceSurfaces() {
	guiSurface_t surf;
	if(mSurfaces.size() > 0) {
		surf = *mpActSurf;
	} else {
		surf.bAlpha = false;
		surf.tex = HTexture();
	}

	// make the pointers 16 byte aligned
	mNumIndices = RIALIGN(mNumIndices, 8);

	surf.firstIndex = mNumIndices;
	surf.numIndices = 0;
	surf.numVertices = 0;

	mSurfaces.push_back(surf);
	mpActSurf = &mSurfaces[mSurfaces.size()-1];
}

void D92DRenderer::OnLostDevice() {
	assert(!mbLocked);
	Log("D92DRenderer::OnLostDevice(): Releasing Buffers.\n");

	SAFE_RELEASE(mpVB);
	SAFE_RELEASE(mpIB);

	Clear();
	mNumVerts	= 0;
	mNumIndices = 0;
}

bool D92DRenderer::OnRestore(LPDIRECT3DDEVICE9 pDevice, const D9TextureManager *pTexMgr) {
	Log("D92DRenderer::OnRestore: Restoring Buffers.\n");
	assert(!mbLocked);
	assert(!mpVB && !mpIB);
	assert(pDevice && pTexMgr);
	mpDevice = pDevice;
	mpTexMgr = pTexMgr;

	HRRELEASE(mpDevice->CreateVertexBuffer(mVBSize, D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, 
										   RIFVF_GUIVERT, D3DPOOL_DEFAULT, &mpVB, NULL));
	HRRELEASE(mpDevice->CreateIndexBuffer(mIBSize, D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
										  D3DFMT_INDEX16, D3DPOOL_DEFAULT, &mpIB, NULL));

	mbLocked	= false;
	mNumVerts	= 0;
	mNumIndices = 0;
	mpLockedVerts	= NULL;
	mpLockedIndices = NULL;
	Clear();

	return true;
}

/* -------------------------------------------------------- */
bool D92DRenderer::BeginFrame() {
	assert(!mbLocked && "D92DRenderer: Call the methods in the right order!");
	assert(mpDevice);

	Lock();
	Clear();

	return true;
}

void D92DRenderer::EndFrame() {
	assert(mpDevice);
	assert(mpTexMgr);

	Unlock();

	mpDevice->SetFVF(RIFVF_GUIVERT);
	mpDevice->SetStreamSource(0, mpVB, 0, sizeof(guiVert_t));
	mpDevice->SetIndices(mpIB);

	U32 minVertIndex = 0;
	for(U32 i=0; i<mSurfaces.size(); ++i) {
		guiSurface_t surf = mSurfaces[i];
		
		const LPDIRECT3DTEXTURE9 pTex = mpTexMgr->Get(surf.tex)->GetData();
		mpDevice->SetTexture(0, pTex);

		if(surf.bAlpha) {
			mpDevice->SetRenderState(D3DRS_ALPHABLENDENABLE , TRUE);
			mpDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			mpDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		} else {
			mpDevice->SetRenderState(D3DRS_ALPHABLENDENABLE , FALSE);
		}

		mpDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, minVertIndex, surf.numVertices, surf.firstIndex, surf.numIndices/3);
		minVertIndex += surf.numVertices;
	}
}

guiVert_t* D92DRenderer::Alloc(U32 numVerts, const index_t *pIndices, U32 numIndices, HTexture tex, 
							   bool bAlpha) {
	assert(mpDevice && mpVB && mpIB && pIndices && "D92DRenderer::Alloc");
	assert(mbLocked);

	if(mNumIndices + numIndices + 8 > MAX_INDICES) {	// ALIGN
		LogE("D92DRenderer::Alloc: MAX_INDICES exceeded!\n");
		return NULL;
	}

	if(mNumVerts + numVerts > MAX_VERTS) {
		LogE("D92DRenderer::Alloc: MAX_VERTICES exceeded!\n");
		return NULL;
	}

	if(mpActSurf->tex != tex || mpActSurf->bAlpha != bAlpha) {
		if(mpActSurf->numIndices)
			AdvanceSurfaces();

		mpActSurf->tex = tex;
		mpActSurf->bAlpha = bAlpha;
	}

	U32 startVerts = mNumVerts;
	U32 startIndices = mNumIndices;

	mNumVerts	+= numVerts;
	mNumIndices += numIndices;

	mpActSurf->numVertices += numVerts;
	mpActSurf->numIndices += numIndices;

	// write indices
	for(U32 i=0; i<numIndices; ++i)					// optimize?
		mpLockedIndices[startIndices + i] = pIndices[i] + static_cast<index_t>(startVerts);


	return mpLockedVerts + startVerts;
}




void D92DRenderer::Lock() {
	assert(!mbLocked);
	assert(mpVB && mpIB);

	mNumVerts	= 0;
	mNumIndices = 0;

	HR(mpVB->Lock(0, 0, reinterpret_cast<void**>(&mpLockedVerts), D3DLOCK_DISCARD));
	HR(mpIB->Lock(0, 0, reinterpret_cast<void**>(&mpLockedIndices), D3DLOCK_DISCARD));

	mbLocked = true;
}

void D92DRenderer::Unlock() {
	assert(mbLocked);
	assert(mpVB && mpIB);

	HR(mpVB->Unlock());
	HR(mpIB->Unlock());

	mpLockedVerts   = NULL;
	mpLockedIndices = NULL;
	mbLocked		= false;
}



} // namespace ri