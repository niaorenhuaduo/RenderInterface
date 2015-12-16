#include "..\Global.h"

#include "StackAllocator.h"
#include "..\Debugging\Logger.h"
#include <type_traits>

StackAllocator::StackAllocator() : mName(""), mSizeInBytes(0), mpStartMemory(NULL), mAllocatedInBytes(0), mActPosition(0) {}
StackAllocator::~StackAllocator() { 
	FLOG(logINFO) << "Destroying StackAllocator: " << mName.c_str();
	OnRelease(); 
}

void StackAllocator::OnRelease() {
	if(mpStartMemory) {
		free(mpStartMemory);
		mpStartMemory = NULL;
	}
	mSizeInBytes	  = 0;
	mAllocatedInBytes = 0;
	mActPosition	  = 0;
}

const U32 gMaxAlignment = std::tr1::alignment_of<std::max_align_t>::value;
void* StackAllocator::Alloc(const U32 sizeInBytes) {
	assert(mpStartMemory && "StackAllocator not initialized.");
	U32 alignment = gMaxAlignment;

	void *pAddress = AllocateAligned(sizeInBytes, alignment);
	return pAddress;
}

HRESULT StackAllocator::OnInit(const char* const pName, const U32 sizeInBytes) {
	mName = pName;
	if(mpStartMemory) {
		FLOG(logWARNING) << "Reinitializing StackAllocator: " << mName.c_str();
		OnRelease();
	} else { FLOG(logINFO) << "Initializing StackAllocator: " << mName.c_str() << "; Size in Bytes: " << sizeInBytes; }

	mSizeInBytes = sizeInBytes;
	mpStartMemory = malloc(mSizeInBytes);
	if(!mpStartMemory) {
		FLOG(logERROR) << "StackAllocator: Out of memory!";
		assert(0 && "Out of memory");
		return E_FAIL;
	}

	mAllocatedInBytes = 0;
	mActPosition = reinterpret_cast<U32>(mpStartMemory);

	return S_OK;
}

void* StackAllocator::AllocateAligned(const U32 sizeInBytes, const U32 alignment) {
	assert(alignment > 1 && "StackAllocator: Alignment must be greater than one.");
	U32 expandedSize = sizeInBytes + alignment;

	U32 rawAddress = reinterpret_cast<U32>(AllocateUnaligned(expandedSize));

	U32 mask = (alignment - 1);
	U32 misalignment = (rawAddress & mask);
	U32 adjustment = alignment - misalignment;

	U32 alignedAddress = rawAddress + adjustment;
	U8* pAdjustment = reinterpret_cast<U8*>(alignedAddress - 1);	// Store the adjustment in the byte before the address.. not necessary in stackallocator
	*pAdjustment = static_cast<U8>(adjustment);
	return reinterpret_cast<void*>(alignedAddress);
}

void* StackAllocator::AllocateUnaligned(const U32 sizeInBytes) {
	if(mAllocatedInBytes + sizeInBytes > mSizeInBytes) {
		FLOG(logERROR) << "StackAllocator: Overrun in StackAllocator: " << mName.c_str();
		assert(0 && "StackAllocator: Overrun");
		return NULL;
	}

	void* returnAddress = reinterpret_cast<void*>(mActPosition);

	mAllocatedInBytes += sizeInBytes;
	mActPosition	  += sizeInBytes;

	return returnAddress;
}

const StackMarker StackAllocator::GetMarker() const {
	if(mpStartMemory) return mActPosition;
	else return MAXUINT;
}

void StackAllocator::FreeToMarker(const StackMarker marker) {
	if(marker > mActPosition) return;

	U32 startAddress  = reinterpret_cast<U32>(mpStartMemory);
	mActPosition	  = marker;
	mAllocatedInBytes = mActPosition - startAddress;
}


// Double buffered
HRESULT DoubleBufferedAllocator::OnInit(const char* const pName,const U32 sizeInBytesPerBuffer) {
	if(FAILED(mStacks[0].OnInit(pName, sizeInBytesPerBuffer)))
		return E_FAIL;
	if(FAILED(mStacks[1].OnInit(pName, sizeInBytesPerBuffer)))
		return E_FAIL;

	mActStack = 0;
	return S_OK;
}