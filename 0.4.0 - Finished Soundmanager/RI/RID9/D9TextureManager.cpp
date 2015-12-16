#include "DXGlobal.h"
#include "D9TextureManager.h"

#include "..\..\Include\HandleMgr.h"

#include <DxErr.h>
#include <d3dx9.h>





namespace ri
{



/*----------------------------------------------------------------*/
// D9TextureManager
/*----------------------------------------------------------------*/
D9TextureManager::D9TextureManager(LPDIRECT3DDEVICE9 pDevice, const std::string &fileDirectory) :
	ITextureManager(fileDirectory),
	mpDevice(pDevice),
	mpDefaultTexture(NULL)
{
	assert(pDevice);

	// Load default-Texture
	mpDefaultTexture = rnew D9FileTexture(std::string(mFileDirectory) + "default.dds");
	if(!mpDefaultTexture->OnRestore(pDevice)) {
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

	D9Texture *pTex = rnew D9FileTexture(name);
	assert(pTex && "Out of memory!");
	if(!pTex->OnRestore(mpDevice)) {
		SAFE_DELETE(pTex);
		Log("D9TextureManager: Replacing the Texture with the default-Texture.\n");
		handle.Init(HTexture::TEX_DEFAULT);
		return mpDefaultTexture;
	}

	AddTextureToData(handle, pTex);

	return mData[handle.GetIndex()];
}


ri::HTexture D9TextureManager::LoadTextureFromImage( const Image &image ) {
	assert(mpDevice);

	HTexture handle;

	D9ImageTexture *pTex = rnew D9ImageTexture(image);
	if(!pTex->OnRestore(mpDevice)) {
		SAFE_DELETE(pTex);
		Log("D9TextureManager: Replacing the Texture with the default-Texture.\n");
		handle.Init(HTexture::TEX_DEFAULT);
		return handle;
	}

	AddTextureToData(handle, pTex);

	return handle;
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

bool D9TextureManager::OnLostDevice() {
	Log("D9TextureManager::OnLostDevice: Releasing Textures.\n");
	TextureVec::iterator it = mData.begin();
	while(it != mData.end()) {
		if(*it) 
			(*it)->OnLostDevice();
		++it;
	}

	mpDefaultTexture->OnLostDevice();

	return true;
}

bool D9TextureManager::OnRestore(LPDIRECT3DDEVICE9 pDevice) {
	assert(pDevice);
	Log("D9TextureManager::OnRestore: Restoring Textures.\n");

	if(!mpDefaultTexture->OnRestore(mpDevice)) {
		assert(0 && "D9TextureManager::OnRestore: Failed to restore default-Texture.");
		return false;
	}

	TextureVec::iterator it = mData.begin();
	while(it != mData.end()) {
		if(*it) {
			if(!(*it)->OnRestore(mpDevice)) {
				assert(0 && "D9TextureManager::OnRestore: This shouldn't happen!");
			}
		}

		++it;
	}

	return true;
}


void D9TextureManager::AddTextureToData( HTexture &handle, D9Texture *pTex )
{
	assert(pTex);

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
}


} // namespace ri