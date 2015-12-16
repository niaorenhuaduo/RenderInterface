#pragma once

#ifndef UNICODE
#define UNICODE
#endif

#include <RI.h>
#include "LogFile.h"

// Activate debug information
#if defined(_DEBUG)
	#define D3D_DEBUG_INFO
#endif

#include <d3d9.h>
#include <d3dx9.h>
#include <DxErr.h>

// DXTRACE HELPER
#if defined(DEBUG) | defined(_DEBUG)
	#ifndef HR
	#define HR(x)                                      \
	{                                                  \
		HRESULT hr = x;                                \
		if(FAILED(hr))                                 \
		{                                              \
			DXTrace(__FILE__, __LINE__, hr, TEXT(#x), TRUE); \
		}                                              \
	}
	#endif

#else
	#ifndef HR
	#define HR(x) x;
	#endif
#endif 

// Can be used in release mode...for initialization
#define HRRELEASE(x)                                      \
{                                                  \
	HRESULT hr = x;                                \
	if(FAILED(hr))                                 \
	{                                              \
		DXTrace(__FILE__, __LINE__, hr, TEXT(#x), TRUE); \
	}                                              \
}
