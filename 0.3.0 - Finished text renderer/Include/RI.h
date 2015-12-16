#pragma once


/********************************************************************/
// INCLUDES
/********************************************************************/
#define NOMINMAX
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



/********************************************************************/
// DEFINES
/********************************************************************/
#define RIALIGN( x, a ) ( ( ( x ) + ((a)-1) ) & ~((a)-1) )

#define SAFE_DELETE(a)			if( (a) != NULL ) delete (a); (a) = NULL;
#define SAFE_DELETE_ARRAY(a)	if( (a) != NULL ) delete[] (a); (a) = NULL;
#define SAFE_RELEASE(a)			if( (a) != NULL ) (a)->Release(); (a) = NULL;

// Exporting defines
#ifdef RI_EXPORTS
#define RI_EXPORT __declspec(dllexport) 
#else
#define RI_EXPORT __declspec(dllimport) 
#endif




/********************************************************************/
// MEMORYLEAK LOGGER
/********************************************************************/
#ifdef _DEBUG
#pragma comment(lib, "vld.lib")
#include <vld.h>
#endif

#include <crtdbg.h>
#ifdef _DEBUG
#	define rnew new(_NORMAL_BLOCK,__FILE__, __LINE__)
#else
#	define rnew new
#endif
// =================== //




/********************************************************************/
// TYPE DEFINITIONS
/********************************************************************/
typedef uint8_t  U8;
typedef int8_t   I8;
typedef uint16_t U16;
typedef int16_t  I16;
typedef uint32_t U32;
typedef int32_t  I32;
typedef uint64_t U64;
typedef int64_t  I64;


////////////////////////////////////////////////////////////
/// \brief Classes that privately derive from this one can't be copied
///
////////////////////////////////////////////////////////////
class RI_EXPORT UnCopyable {
protected:
	UnCopyable() {}

private:
	UnCopyable(const UnCopyable &rhs);
	UnCopyable& operator=(const UnCopyable &rhs);
};




/********************************************************************/
// POST GLOBAL INCLUDES
/********************************************************************/
#include "RIMath.h"
/*
#include "RIColor.h"
#include "RIMath.h"
#include "RIVec.h"
#include "RIMatrix.h"
*/
