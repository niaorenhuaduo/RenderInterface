#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#ifndef UNICODE
#define UNICODE
#endif

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <stdint.h>
#include <assert.h>

#include <algorithm>
#include <string>
#include <list>
#include <vector>
#include <queue>
#include <map>

//#include "..\3rdParty\tinyxml2\tinyxml2.h"

// memory leaks
#include <crtdbg.h>
#ifdef _DEBUG
#	define rnew new(_NORMAL_BLOCK,__FILE__, __LINE__)
#else
#	define rnew new
#endif

// TYPEDEFS
typedef uint8_t  U8;
typedef int8_t   I8;
typedef uint16_t U16;
typedef int16_t  I16;
typedef uint32_t U32;
typedef int32_t  I32;
typedef uint64_t U64;
typedef int64_t  I64;

#define RIALIGN( x, a ) ( ( ( x ) + ((a)-1) ) & ~((a)-1) )

#define SAFE_DELETE(a)			if( (a) != NULL ) delete (a); (a) = NULL;
#define SAFE_DELETE_ARRAY(a)	if( (a) != NULL ) delete[] (a); (a) = NULL;
#define SAFE_RELEASE(a)			if( (a) != NULL ) (a)->Release(); (a) = NULL;

template <typename T>
struct RIRect {
	RIRect() : left(), top(), width(), height() {}
	RIRect(T _left, T _top, T _width, T _height) : left(_left), top(_top), width(_width), height(_height) {}

	T left;
	T top;
	T width;
	T height;
};

typedef RIRect<I32>   RIIntRect;
typedef RIRect<float> RIFloatRect;

class UnCopyable {
public:
	UnCopyable() {}
	~UnCopyable() {}

private:
	UnCopyable(const UnCopyable &rhs);
	UnCopyable& operator=(const UnCopyable &rhs);
};

// post global include
#include "RIColor.h"
#include "RIMath.h"
#include "RIVec.h"
#include "RIMatrix.h"
