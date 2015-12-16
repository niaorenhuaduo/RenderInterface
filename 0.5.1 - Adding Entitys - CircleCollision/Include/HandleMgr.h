#pragma once

/********************************************************************/
// INCLUDES
/********************************************************************/
#include "RI.h"
#include <assert.h>
#include <vector>
#include <cassert>
#include <boost\iterator\filter_iterator.hpp>



namespace ri
{

////////////////////////////////////////////////////////////
/// \brief A simple unsigned int used as a texture handle
///
////////////////////////////////////////////////////////////
class RI_EXPORT HTexture {
public:
	////////////////////////////////////////////////////////////
	/// \brief Uninitialized handle
	///
	////////////////////////////////////////////////////////////
	HTexture() : mHandle(0) {}
	~HTexture() {}
	
	////////////////////////////////////////////////////////////
	/// \brief Copy constructor/assignment
	///
	////////////////////////////////////////////////////////////
	HTexture(const HTexture &rhs) { mHandle = rhs.GetHandle(); }
	HTexture& operator=(const HTexture &rhs) { mHandle = rhs.GetHandle(); return *this; }

	////////////////////////////////////////////////////////////
	/// \brief This is the only way to initialize a new handle
	///
	////////////////////////////////////////////////////////////
	void Init(U32 index);	

	// Set/get
	U16 GetIndex() const	{ return s.mIndex; }
	U16 GetMagic() const	{ return s.mMagic; }
	U32 GetHandle() const	{ return mHandle; }
	bool IsNull() const		{ return !mHandle; }

	operator U32() const	{ return mHandle; }


	enum {
		// sizes for bitfields
		MAX_BITS_INDEX = 16,
		MAX_BITS_MAGIC = 16,

		TEX_DEFAULT = (1 << MAX_BITS_INDEX) - 2,
		MAX_INDEX   = (1 << MAX_BITS_INDEX) - 1,
		MAX_MAGIC   = (1 << MAX_BITS_MAGIC) - 1,
	};
private:
	union {
		struct {
			unsigned mIndex : MAX_BITS_INDEX;	// index into resource
			unsigned mMagic : MAX_BITS_MAGIC;	// validation
		} s;
		U32 mHandle;
	};
};


inline void HTexture::Init(U32 index) {
	assert(IsNull());
	assert(index < MAX_INDEX);

	static U32 sMagicCounter = 0;
	if(++sMagicCounter > MAX_MAGIC)
		sMagicCounter = 1;

	s.mIndex = index;
	s.mMagic = sMagicCounter;
}


/*
inline bool operator!=(HTexture l, HTexture r) {
	return (l.GetHandle() != r.GetHandle());	}

inline bool operator==(HTexture l, HTexture r) {
	return (l.GetHandle() == r.GetHandle());	}*/




////////////////////////////////////////////////////////////
/// \brief Handle Manager for a specific type of data
///
/// When using the iterator:
/// The class used as DATA has to implement an IsInitialized method.
/// Form: bool IsInitialized()
///
/// \todo: Look into game programming gems 1 and update method explanations
///
////////////////////////////////////////////////////////////
template<typename DATA, typename HANDLE>
class HandleMgr : private UnCopyable {
public:
	HandleMgr() {}
	~HandleMgr() {}

	DATA* Acquire(HANDLE &handle);
	void  Release(HANDLE handle);

	DATA*	    Get(HANDLE handle);
	const DATA* Get(HANDLE handle) const;

	U32  NumUsedHandles() const { return (mMagicNumbers.size() - mFreeSlots.size());}
	bool HasUsedHandles() const { return !!NumUsedHandles(); }
private:
	typedef std::vector<DATA>	DataVec;	// Contains the data
	typedef std::vector<U32>	MagicVec;	// Validation numbers
	typedef std::vector<U32>	FreeVec;	// indices to free spots in DataVec

	DataVec  mData;
	MagicVec mMagicNumbers;
	FreeVec	 mFreeSlots;


public:		// iterator over the data
	struct IsDataValid {
		bool operator()(DATA &data) { return data.IsInitialized(); }
	};

	typedef boost::filter_iterator<IsDataValid, typename std::vector<DATA>::iterator> iterator;
	iterator begin() { return boost::make_filter_iterator<IsDataValid>(mData.begin(), mData.end()); }
	iterator end()   { return boost::make_filter_iterator<IsDataValid>(mData.end(), mData.end()); }
};

template<typename DATA, typename HANDLE>
DATA* HandleMgr<DATA, HANDLE>::Acquire(HANDLE &handle) {
	U32 index = 0;
	if(mFreeSlots.empty()) {
		// Add new entry
		index = mMagicNumbers.size();
		handle.Init(index);
		mMagicNumbers.push_back(handle.GetMagic());
		mData.push_back(DATA());
	} else { 
		// use the Freed Resource; 
		index = mFreeSlots.back();
		mFreeSlots.pop_back();
		handle.Init(index);
		mMagicNumbers[index] = handle.GetMagic();
	}

	return &mData[index];
}

template<typename DATA, typename HANDLE>
void HandleMgr<DATA, HANDLE>::Release(HANDLE handle) {
	U32 index = handle.GetIndex();

	// validation check
	assert(index < mData.size());
	assert(handle.GetMagic() == mMagicNumbers[index]);

	mFreeSlots.push_back(index);
	mMagicNumbers[index] = 0;
}

template<typename DATA, typename HANDLE>
inline DATA* HandleMgr<DATA, HANDLE>::Get(HANDLE handle) {
	if(handle.IsNull()) return NULL;

	// validation check
	U32 index = handle.GetIndex();
	if(index >= mData.size() || mMagicNumbers[index] != handle.GetMagic()) {
		assert(0 && "HandleMgr::Get: Invalid handle!");
		return NULL;
	}

	return &mData[index];
}

template<typename DATA, typename HANDLE>
inline const DATA* HandleMgr<DATA, HANDLE>::Get(HANDLE handle) const {
	typedef HandleMgr<DATA, HANDLE> This;
	return (const_cast<This*>(this)->Get(handle));
}



} // namespace ri