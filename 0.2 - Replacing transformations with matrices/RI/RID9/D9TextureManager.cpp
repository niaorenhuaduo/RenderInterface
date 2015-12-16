#include "DXGlobal.h"
#include "D9TextureManager.h"

#include "..\..\Include\RIHandleMgr.h"

#include <DxErr.h>
#include <d3dx9.h>

#include <fstream>

/*----------------------------------------------------------------*/
// D9Texture
/*----------------------------------------------------------------*/
D9Texture::D9Texture(const std::string &name) :
	mFileName(name),
	mWidth(0),
	mHeight(0),
	mNumMipmaps(0),
	mFmt(),
	mdwColorKey(RISTANDARDCOLORKEY),
	mpTexture(NULL),
	mpData(NULL),
	mSize(0)
{	assert(mFileName.size() > 0 && "D9Texture: No Texture Name!");	}

D9Texture::~D9Texture() {
	Log("D9Texture: Releasing Texture and the copy in memory: %s.\n", mFileName.c_str());
	SAFE_RELEASE(mpTexture);

	if(mpData) {
		free(mpData);
		mpData = NULL;
	}
	mSize = 0;
}

HRESULT D9Texture::LoadFromFile() {
	assert(mFileName.size() > 0 && "D9Texture: No Texture Name!");
	Log("D9Texture: Loading into Memory...\n");

	std::ifstream pFile(mFileName.c_str(), std::ios::in | std::ios::binary);
	if(!pFile) {
		LogE("D9Texture: Couldn't find Texturefile.\n");
		mpData = NULL;
		mSize = 0;
		return E_FAIL;
	}

	pFile.seekg(0, std::ios::end);
	mSize = static_cast<U64>(pFile.tellg());
	pFile.seekg(0, std::ios::beg);
	mpData = reinterpret_cast<char*>(malloc(static_cast<size_t>(mSize)));
	if(!mpData) {
		LogE("D9Texture: Out of ram-memory!\n");
		mpData = NULL;
		mSize = 0;
		return E_FAIL;
	}
	pFile.read(mpData, mSize);
	pFile.close();

	return S_OK;
}

HRESULT D9Texture::OnRestore(LPDIRECT3DDEVICE9 pDevice) {
		assert(pDevice);
	SAFE_RELEASE(mpTexture);

	Log("[D9Texture: Restore %s]\n", mFileName.c_str());

	if(mSize == 0) {
		if(FAILED(LoadFromFile()))
			return E_FAIL;
	}

	assert(mpData && mSize > 0);

	D3DXIMAGE_INFO info;
	if(FAILED(D3DXGetImageInfoFromFileInMemory(mpData, static_cast<U32>(mSize), &info))) {
		LogE("D9Texture: Failed to load Texture information.\n");
		mpTexture = NULL;
		if(mpData) free(mpData);
		mpData = NULL;
		mSize = 0;
		return E_FAIL;
	}

	Log("D9Texture: Loading into VRam.\n");
	if(FAILED(D3DXCreateTextureFromFileInMemoryEx(pDevice, mpData, static_cast<U32>(mSize),  info.Width,  info.Height, 1,
										0, D3DFMT_UNKNOWN , D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT,
										mdwColorKey, &info, NULL, &mpTexture))) 
	{
		LogE("D9Texture: Failed to load the Texture into VRAM.\n");
		mpTexture = NULL;
		if(mpData) free(mpData);
		mpData = NULL;
		mSize = 0;
		return E_FAIL;		
	}

	mWidth = info.Width;
	mHeight = info.Height;
	mFmt = info.Format;
	mNumMipmaps = info.MipLevels;
	Log("[D9Texture: Restore OK]\n");

	return S_OK;
}

void D9Texture::OnLostDevice() {
	Log("D9Texture: Release %s.\n", mFileName.c_str());
	SAFE_RELEASE(mpTexture);
}



/*----------------------------------------------------------------*/
// D9TextureManager
/*----------------------------------------------------------------*/
D9TextureManager::D9TextureManager(LPDIRECT3DDEVICE9 pDevice, const std::string &fileDirectory) :
	RITextureManager(fileDirectory),
	mpDevice(pDevice),
	mpDefaultTexture(NULL)
{
	assert(pDevice);

	// Load default-Texture
	mpDefaultTexture = rnew D9Texture(mFileDirectory + "default.dds");
	if(FAILED(mpDefaultTexture->OnRestore(pDevice))) {
		assert(0 && "Couldn't find the default-Texture!");
	}

	Log("--D9TextureManager created!--\n");
}

D9TextureManager::~D9TextureManager() {
	TextureVec::iterator it = mData.begin();
	while(it != mData.end()) {
		if(*it) SAFE_DELETE(*it);
		++it;
	}

	SAFE_DELETE(mpDefaultTexture);

	Log("--D9TextureManager deleted!--\n");
}


// Helper
const D9Texture* D9TextureManager::Create(HTexture &handle, const std::string &name) {
	assert(mpDevice);

	D9Texture *pTex = rnew D9Texture(name);
	assert(pTex && "Out of memory!");
	if(FAILED(pTex->OnRestore(mpDevice))) {
		SAFE_DELETE(pTex);
		Log("D9TextureManager: Replacing the Texture with the default-Texture.\n");
		handle.Init(HTexture::TEX_DEFAULT);
		return mpDefaultTexture;
	}

	U32 index = 0;
	if(mFreeIndices.size() > 0) {
		index = mFreeIndices.back();
		mFreeIndices.pop_back();
		handle.Init(index);
		mMagicNumbers[index] = handle.GetMagic();
		
		assert(mData[index] == NULL && "One Resource didn't get properly destroyed!");
		mData[index] = pTex;
	} else {
		index = mMagicNumbers.size();
		handle.Init(index);
		mMagicNumbers.push_back(handle.GetMagic());
		mData.push_back(pTex);
	}

	return mData[index];
}

void D9TextureManager::Release(HTexture handle) {
	U32 index = handle.GetIndex();
	if(index == HTexture::TEX_DEFAULT) return;

	assert(index < mMagicNumbers.size());
	assert(handle.GetMagic() == mMagicNumbers[index]);

	// Delete object
	assert(mData[index] && "Trying to release a destroyed object!");
	SAFE_DELETE(mData[index]);

	// Remove from lists
	mFreeIndices.push_back(index);
	mMagicNumbers[index] = 0;
	assert(mData[index] == NULL);
}

HRESULT D9TextureManager::OnLostDevice() {
	Log("D9TextureManager::OnLostDevice: Releasing Textures.\n");
	TextureVec::iterator it = mData.begin();
	while(it != mData.end()) {
		if(*it) 
			(*it)->OnLostDevice();
		++it;
	}

	mpDefaultTexture->OnLostDevice();

	return S_OK;
}

HRESULT D9TextureManager::OnRestore(LPDIRECT3DDEVICE9 pDevice) {
	assert(pDevice);
	Log("D9TextureManager::OnRestore: Restoring Textures.\n");

	if(FAILED(mpDefaultTexture->OnRestore(mpDevice))) {
		assert(0 && "D9TextureManager::OnRestore: Failed to restore default-Texture.");
		return E_FAIL;
	}

	TextureVec::iterator it = mData.begin();
	while(it != mData.end()) {
		if(*it) {
			if(FAILED((*it)->OnRestore(mpDevice))) {
				assert(0 && "D9TextureManager::OnRestore: This shouldn't happen!");
			}
		}

		++it;
	}

	return S_OK;
}
