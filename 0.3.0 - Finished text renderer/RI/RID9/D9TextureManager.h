#pragma once

/********************************************************************/
// INCLUDES
/********************************************************************/
#include "DXGlobal.h"
#include <IRenderSystem.h>


// TODO: Set Mipmap filter when releasing
// TODO: D9TextureManager::ReleaseTexture: Doesn't remove original text-string when releasing a DefaultTexture


namespace ri
{


////////////////////////////////////////////////////////////
/// \brief A base class for DirectX 9 textures
///
////////////////////////////////////////////////////////////
class D9Texture : private UnCopyable {
public:
	D9Texture();
	virtual ~D9Texture();

	virtual bool OnRestore(LPDIRECT3DDEVICE9 pDevice)=0;
	virtual void OnLostDevice();

	// Set/Get
	const LPDIRECT3DTEXTURE9 GetData() const	{ return mpTexture; }
	DWORD	GetColorKey() const			{ return mdwColorKey; }
	U32		GetWidth() const			{ return mWidth; }
	U32		GetHeight() const			{ return mHeight; }

	virtual const std::string& GetName() const=0;


protected:
	LPDIRECT3DTEXTURE9	mpTexture;
	U32					mWidth;
	U32					mHeight;
	D3DFORMAT			mFmt;
	DWORD				mdwColorKey;
	U32					mNumMipmaps;

};

////////////////////////////////////////////////////////////
/// \brief A DirectX9 texture loaded from file
///
////////////////////////////////////////////////////////////
class D9FileTexture : public D9Texture {
public:
	D9FileTexture(const std::string &fileName);
	virtual ~D9FileTexture();

	bool OnRestore(LPDIRECT3DDEVICE9 pDevice);
	
	const std::string& GetName() const { return mFileName; }

private:
	std::string mFileName;

	char *mpData;
	U64   mSize;

private:
	bool LoadFromFile();
};

////////////////////////////////////////////////////////////
/// \brief A managed DirectX9 texture loaded from an image. Doesn't require a reset when the device is lost
///
////////////////////////////////////////////////////////////
class D9ImageTexture : public D9Texture {
public:
	D9ImageTexture(const Image &image);
	virtual ~D9ImageTexture();

	////////////////////////////////////////////////////////////
	/// \brief Creates the texture from the image
	///
	/// Call this right after the constructor. The image reference has to be valid on the first call
	/// Everything else results in undefined behavior
	///
	////////////////////////////////////////////////////////////
	bool OnRestore(LPDIRECT3DDEVICE9 pDevice);
	void OnLostDevice() {}

	const std::string& GetName() const { return mName; }

private:
	const Image &mTempImage;

	const static std::string mName;
};


/*----------------------------------------------------------------*/
// D9TextureManager
/*----------------------------------------------------------------*/
class D9TextureManager : public ITextureManager {
public:
	D9TextureManager(LPDIRECT3DDEVICE9 pDevice, const std::string &fileDirectory);
	~D9TextureManager();

	HTexture GetTexture(const std::string &name);
	HTexture LoadTextureFromImage(const Image &image);
	void	 ReleaseTexture(HTexture handle);


	const D9Texture* Get(HTexture handle) const;
	D9Texture*		 Get(HTexture handle);

	bool OnLostDevice();
	bool OnRestore(LPDIRECT3DDEVICE9 pDevice);

	U32 GetWidth(HTexture handle) const						{ return Get(handle)->GetWidth(); }
	U32 GetHeight(HTexture handle) const					{ return Get(handle)->GetHeight(); }
	U32 GetColorKey(HTexture handle) const					{ return Get(handle)->GetColorKey(); }
	const std::string& GetName(HTexture handle) const		{ return Get(handle)->GetName(); }

private:
	typedef std::vector<D9Texture*> TextureVec;
	typedef std::vector<U32>		MagicVec;
	typedef std::vector<U32>		FreeVec;

	// compare strings
	struct str_less {
		bool operator()(const std::string &l, const std::string &r) const
			{ return (::_stricmp(l.c_str(), r.c_str()) < 0 ); }
	};

	typedef std::map<std::string, HTexture, str_less> NameIndex;
	typedef std::pair<NameIndex::iterator, bool>	  NameIndexRes;


	// data
	NameIndex	mNameIndex;

	TextureVec	mData;
	MagicVec	mMagicNumbers;
	FreeVec		mFreeIndices;

	D9Texture		 *mpDefaultTexture;
	LPDIRECT3DDEVICE9 mpDevice;

private:
	const D9Texture* Create(HTexture &handle, const std::string &name);
	void			 Release(HTexture handle);
	void			 AddTextureToData(HTexture &handle, D9Texture *pTex);
};


inline HTexture D9TextureManager::GetTexture(const std::string &name) {
	assert(name.length() <= 200 && "TextureManager::VGetTextures: the length of the name string is too long!");

	std::string filePath = mFileDirectory + name;
	NameIndexRes r = mNameIndex.insert(std::make_pair(filePath, HTexture()));
	if(r.second) {
		Log("D9TextureManager: Add '%s' to the Manager.\n", filePath.c_str());
		const D9Texture* tex = Create(r.first->second, filePath);
		assert(tex);
	}

	return r.first->second;
}

inline void D9TextureManager::ReleaseTexture(HTexture handle) {
	D9Texture *pTex = Get(handle);
	if(pTex && pTex != mpDefaultTexture) {
		Log("D9TextureManager: Remove '%s' from the Manager.\n", pTex->GetName().c_str());

		D9FileTexture *pFileTex = dynamic_cast<D9FileTexture*>(pTex);
		if(pFileTex) mNameIndex.erase(mNameIndex.find(pTex->GetName()));

		Release(handle);
	}
}

inline D9Texture* D9TextureManager::Get(HTexture handle) {
	if(handle.IsNull() || handle.GetIndex() == HTexture::TEX_DEFAULT) return mpDefaultTexture;

	U32 index = handle.GetIndex();
	if(index >= mData.size() || mMagicNumbers[index] != handle.GetMagic()) {
		LogE("ITextureManager::Get: Invalid Texture handle! Returning default texture");
		return mpDefaultTexture;
	}

	return mData[index];
}

inline const D9Texture* D9TextureManager::Get(HTexture handle) const {
	return const_cast<D9TextureManager*>(this)->Get(handle);
}



} // namespace ri