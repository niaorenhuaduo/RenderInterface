#pragma once

#include <RIRenderInterface.h>


// TODO: Set Mipmap filter when releasing
// TODO: D9TextureManager::ReleaseTexture: Doesn't remove original text-string when releasing a DefaultTexture


/*----------------------------------------------------------------*/
// D9Texture
/*----------------------------------------------------------------*/
class D9Texture : private UnCopyable {
public:
	D9Texture(const std::string &name);
	~D9Texture();

	HRESULT OnRestore(LPDIRECT3DDEVICE9 pDevice);
	void	OnLostDevice();

	// Set/Get
	const LPDIRECT3DTEXTURE9 GetData() const	{ return mpTexture; }
	DWORD	GetColorKey() const			{ return mdwColorKey; }
	U32		GetWidth() const			{ return mWidth; }
	U32		GetHeight() const			{ return mHeight; }
	const std::string& GetName() const	{ return mFileName; }
private:
	LPDIRECT3DTEXTURE9	mpTexture;
	U32					mWidth;
	U32					mHeight;
	D3DFORMAT			mFmt;
	DWORD				mdwColorKey;
	U32					mNumMipmaps;
	std::string			mFileName;

	char *mpData;	// Bitmap in memory for reload
	U64	  mSize;

private:
	D9Texture() {}
	HRESULT LoadFromFile();
};

/*----------------------------------------------------------------*/
// D9TextureManager
/*----------------------------------------------------------------*/
class D9TextureManager : public RITextureManager {
public:
	D9TextureManager(LPDIRECT3DDEVICE9 pDevice, const std::string &fileDirectory);
	~D9TextureManager();

	HTexture VGetTexture(const std::string &name);
	void	 VReleaseTexture(HTexture handle);

	const D9Texture* Get(HTexture handle) const;
	D9Texture*		 Get(HTexture handle);

	HRESULT OnLostDevice();
	HRESULT OnRestore(LPDIRECT3DDEVICE9 pDevice);

	U32 VGetWidth(HTexture handle) const					{ return Get(handle)->GetWidth(); }
	U32 VGetHeight(HTexture handle) const					{ return Get(handle)->GetHeight(); }
	DWORD VGetColorKey(HTexture handle) const				{ return Get(handle)->GetColorKey(); }
	const std::string& VGetName(HTexture handle) const      { return Get(handle)->GetName(); }

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
};


inline HTexture D9TextureManager::VGetTexture(const std::string &name) {
	assert(name.length() <= 200 && "TextureManager::VGetTextures: the length of the name string is too long!");

	std::string filePath = mFileDirectory + name;
	NameIndexRes r = mNameIndex.insert(std::make_pair(filePath, HTexture()));
	if(r.second) {
		Log("D9TextureManager: Add '%s' to the Manager.\n", filePath);
		const D9Texture* tex = Create(r.first->second, filePath);
		assert(tex);
	}

	return r.first->second;
}

inline void D9TextureManager::VReleaseTexture(HTexture handle) {
	D9Texture *pTex = Get(handle);
	if(pTex) {
		Log("D9TextureManager: Remove '%s' from the Manager.\n", pTex->GetName());
		mNameIndex.erase(mNameIndex.find(pTex->GetName()));
		Release(handle);
	}
}

inline D9Texture* D9TextureManager::Get(HTexture handle) {
	if(handle.IsNull() || handle.GetIndex() == HTexture::TEX_DEFAULT) return mpDefaultTexture;

	U32 index = handle.GetIndex();
	if(index >= mData.size() || mMagicNumbers[index] != handle.GetMagic()) {
		assert(0 && "D9TextureManager::Get: Invalid Handle!");
		return NULL;
	}

	return mData[index];
}

inline const D9Texture* D9TextureManager::Get(HTexture handle) const {
	return const_cast<D9TextureManager*>(this)->Get(handle);
}