#pragma once

// TODO: Alloc: calculation of alignment

typedef U32 StackMarker;

class StackAllocator {
public:
	explicit StackAllocator();
	~StackAllocator();

	HRESULT OnInit(const char* const pName, const U32 sizeInBytes);
	void*   Alloc(const U32 sizeInBytes);		// always returns aligned memory; not necessary to test for nullpointer

	const StackMarker GetMarker() const;
	void FreeToMarker(const StackMarker marker);

	void Clear() { mActPosition = reinterpret_cast<U32>(mpStartMemory);  mAllocatedInBytes = 0; }
private:
	void *mpStartMemory;
	StackMarker mActPosition;

	U32	  mSizeInBytes;
	U32	  mAllocatedInBytes;

	std::string mName;

private:
	void* AllocateAligned(const U32 sizeInBytes, const U32 alignment);
	void* AllocateUnaligned(const U32 sizeInBytes);
	void  OnRelease();
};


// memory allocated in frame i can be used in frame i+1
class DoubleBufferedAllocator {
public:
	DoubleBufferedAllocator() {}
	~DoubleBufferedAllocator() {}

	HRESULT OnInit(const char* const pName, const U32 sizeInBytesPerBuffer);

	void SwapBuffers() { mActStack = !mActStack; }
	void ClearActBuffer() { mStacks[mActStack].Clear(); }
	void* Alloc(const U32 sizeInBytes) { return mStacks[mActStack].Alloc(sizeInBytes); }
private:
	U32			   mActStack;
	StackAllocator mStacks[2];
};