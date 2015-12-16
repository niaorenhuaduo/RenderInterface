#include "DXGlobal.h"
#include "D9TextureManager.h"

#include <fstream>

namespace ri {

const std::string D9ImageTexture::mName = "ImageTexture";

/*----------------------------------------------------------------*/
// D9Texture
/*----------------------------------------------------------------*/
D9Texture::D9Texture() :
	mWidth(0),
	mHeight(0),
	mNumMipmaps(0),
	mFmt(),
	mdwColorKey(STANDARDCOLORKEY),
	mpTexture(NULL)
{}

D9Texture::~D9Texture() {
	SAFE_RELEASE(mpTexture);
}



void D9Texture::OnLostDevice() {
	Log("D9Texture: Release %s.\n", GetName().c_str());
	SAFE_RELEASE(mpTexture);
}



/********************************************************************/
// D9FileTexture
/********************************************************************/
D9FileTexture::D9FileTexture(const std::string &fileName) : D9Texture(), mFileName(fileName), mpData(NULL), mSize(0)
{
	assert(mFileName.size() > 0 && "D9Texture: No Texture Name!");
}

D9FileTexture::~D9FileTexture()
{
	Log("D9Texture: Releasing Texture and memory data from: %s.\n", GetName().c_str());

	if(mpData) {
		free(mpData);
		mpData = NULL;
	}
	mSize = 0;
}



bool D9FileTexture::LoadFromFile() {
	Log("D9FileTexture: Loading into Memory...\n");

	std::ifstream pFile(mFileName.c_str(), std::ios::in | std::ios::binary);
	if(!pFile) {
		LogE("D9FileTexture: Couldn't find Texturefile.\n");
		mpData = NULL;
		mSize = 0;
		return false;
	}

	pFile.seekg(0, std::ios::end);
	mSize = static_cast<U64>(pFile.tellg());
	pFile.seekg(0, std::ios::beg);
	mpData = reinterpret_cast<char*>(malloc(static_cast<size_t>(mSize)));
	if(!mpData) {
		LogE("D9FileTexture: Out of ram-memory!\n");
		mpData = NULL;
		mSize = 0;
		return false;
	}
	pFile.read(mpData, mSize);
	pFile.close();

	return true;
}



bool D9FileTexture::OnRestore(LPDIRECT3DDEVICE9 pDevice) {
	assert(pDevice);
	SAFE_RELEASE(mpTexture);

	Log("[D9FileTexture: Restore %s]\n", mFileName.c_str());

	if(mSize == 0) {
		if(!LoadFromFile())
			return false;
	}

	assert(mpData && mSize > 0);

	D3DXIMAGE_INFO info;
	if(FAILED(D3DXGetImageInfoFromFileInMemory(mpData, static_cast<U32>(mSize), &info))) {
		LogE("D9FileTexture: Failed to load Texture information.\n");
		mpTexture = NULL;
		if(mpData) free(mpData);
		mpData = NULL;
		mSize = 0;
		return false;
	}

	Log("D9FileTexture: Loading into VRam.\n");
	if(FAILED(D3DXCreateTextureFromFileInMemoryEx(pDevice, mpData, static_cast<U32>(mSize),  info.Width,  info.Height, 1,
		0, D3DFMT_UNKNOWN , D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT,
		mdwColorKey, &info, NULL, &mpTexture))) 
	{
		LogE("D9FileTexture: Failed to load the Texture into VRAM.\n");
		mpTexture = NULL;
		if(mpData) free(mpData);
		mpData = NULL;
		mSize = 0;
		return false;		
	}


	D3DSURFACE_DESC desc;
	if(FAILED(mpTexture->GetLevelDesc(0, &desc))) {
		LogE("D9ImageTexture: Failed to recieve Texture information");
		SAFE_RELEASE(mpTexture);
		if(mpData) free(mpData);
		mpData = NULL;
		mSize = 0;
		return false;
	}

	mWidth		= desc.Width;
	mHeight		= desc.Height;
	mFmt		= desc.Format;
	mNumMipmaps = info.MipLevels;
	Log("[D9Texture: Restore OK]\n");

	return true;
}



/********************************************************************/
// D9ImageTexture
/********************************************************************/
D9ImageTexture::D9ImageTexture( const Image &image ) : D9Texture(), mTempImage(image)
{}

D9ImageTexture::~D9ImageTexture()
{}

bool D9ImageTexture::OnRestore( LPDIRECT3DDEVICE9 pDevice )
{
	// This is a managed texture. No reset is necessary
	if(mpTexture) return true;
	


	// make sure the parameters are alright
	assert(pDevice);
	assert((mTempImage.GetWidth() != 0 && mTempImage.GetHeight() != 0) && "D9ImageTexture: You passed an uninitialized image!");


	Log("[D9ImageTexture: Create a texture from an image]\n");

	// no mipmaps
	if(FAILED(pDevice->CreateTexture(mTempImage.GetWidth(), mTempImage.GetHeight(), 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &mpTexture, NULL))) {
		LogE("D9ImageTexture: Failed to create the Texture.\n");
		mpTexture = NULL;
		return false;
	}

	D3DSURFACE_DESC desc;
	if(FAILED(mpTexture->GetLevelDesc(0, &desc))) {
		LogE("D9ImageTexture: Failed to recieve Texture information");
		SAFE_RELEASE(mpTexture);
		return false;
	}


	mWidth		= desc.Width;
	mHeight		= desc.Height;
	mNumMipmaps = mpTexture->GetLevelCount();
	mFmt		= desc.Format;



	// Fill the texture
	D3DLOCKED_RECT rect = {0};
	if(FAILED(mpTexture->LockRect(0, &rect, NULL, D3DLOCK_DISCARD))) {
		LogE("D9ImageTexture: Failed to lock the texture");
		SAFE_RELEASE(mpTexture);
		return false;
	}

	unsigned char *pBits = reinterpret_cast<unsigned char*>(rect.pBits);
	const Color *pImageBits = mTempImage.GetPixelPtr();

	for(U32 row=0; row < mHeight; ++row) {
		memcpy(pBits, pImageBits, mWidth * sizeof(Color));

		pBits		+= rect.Pitch;
		pImageBits	+= mTempImage.GetWidth();
	}

	mpTexture->UnlockRect(0);


	return true;
}


} // namespace ri
