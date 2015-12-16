#pragma once

#include "RI.h"
#include "RIHandleMgr.h"


// ================================================
// TODO
// Oben und unten oder links und rechts einen (schwarzen) balken
// zeichnen abhängig von der Auflösung damit überstehende
// sprites nicht mehr sichtbar sind
// ================================================


// Virtual Resolution
const U32 RIVIRTUALXRESOLUTION = 1280;
const U32 RIVIRTUALYRESOLUTION = 800;
const DWORD RISTANDARDCOLORKEY = RICOLOR_ARGB(255, 255, 200, 200);

// Stencil settings
enum stencilTest_t {
	RISTENCIL_DISABLED,
	RISTENCIL_WRITE,
	RISTENCIL_SCISSOR_WRITTEN,
	RISTENCIL_SCISSOR_EMPTY,
};

/*enum guiOrder_t {  needet?
	RIGUIORD_TOPFRONT,
	RIGUIORD_FRONT,
	RIGUIORD_MID,
	RIGUIORD_BACK,
	RIGUIORD_BACKGROUND,
	RIGUIORD_LAST
};*/


struct textureTAG {};
typedef Handle<textureTAG> HTexture;

// For the Logfile
#ifndef _DEBUGFLUSH_
#define _DEBUGFLUSH_
#endif

class RICamera;
class RITextureManager;

/*----------------------------------------------------------------*/
// RenderInterface
/*----------------------------------------------------------------*/
class RIRenderInterface : private UnCopyable {												
public:
	RIRenderInterface() {}		
	virtual ~RIRenderInterface() {}

	// Init/Destroy
	virtual HRESULT VOnInit(const HWND hWnd)=0;
	virtual HRESULT VOnResize(U32 mWidth, U32 mHeight, bool bWindowed)=0;

	virtual HRESULT VBeginRendering()=0;
	virtual void	VClear(bool bRenderT, bool bDepth, bool bStencil)=0;
	virtual void	VDrawPic(const RIVec4 &topleft, const RIVec4 &topright,
							 const RIVec4 &botright, const RIVec4 &botleft,
							 const HTexture tex, 
							 bool bAlpha = false, 
							 stencilTest_t stencil = RISTENCIL_DISABLED)=0; 
	virtual void	VDrawPic(float x, float y, float w, float h, 
							 const HTexture tex, float s1, float t1, float s2, float t2,  
							 bool bAlpha = false,
							 stencilTest_t stencil = RISTENCIL_DISABLED)=0;
	virtual void	VEndRendering()=0;

	// Set/Get
	void SetClearColor(float fR, float fG, float fB) { mClearColor = RIColor(fR, fG, fB); }
	void SetClearColor(const RIColor &color) { mClearColor = color; }
	void SetRunning(bool bRunning) { mbRunning = bRunning; }
	virtual void VSetWindowed(bool bWindowed) { if(mbWindowed == bWindowed) return; VOnResize(mWindowWidth, mWindowHeight, bWindowed); }

	bool IsRunning() const { return mbRunning; }
	bool IsWindowed() const { return mbWindowed; }
	U32  GetWindowWidth() const { return mWindowWidth; }
	U32	 GetWindowHeight() const { return mWindowHeight; }

	RICamera& GetCamera() { assert(mpCamera); return *mpCamera; }					// Use this to configure the game camera
	RITextureManager* GetTexMgr() { return mpTexManager; }
protected:
	RITextureManager *mpTexManager;
	RICamera		 *mpCamera;

	bool		mbRunning;
	U32			mWindowWidth;
	U32			mWindowHeight;
	bool		mbWindowed;
			
	RIColor		mClearColor;
	bool		mbTextureFilter;
	bool		mbMultiSampling;

	HWND		mhWnd;	// Window and DLL-Module
	HINSTANCE	mhDLL;

protected:
	void setWindowSize(U32 width, U32 height)
		{ SetWindowPos(mhWnd, HWND_NOTOPMOST, 0, 0, width, height, SWP_NOACTIVATE | SWP_NOMOVE);	}
};

/*----------------------------------------------------------------*/
// TextureManager
/*----------------------------------------------------------------*/
class RITextureManager : private UnCopyable {
public:
	RITextureManager(const std::string &fileDirectory) { mFileDirectory = fileDirectory; }
	virtual ~RITextureManager() {}

	virtual HTexture VGetTexture(const std::string &name)=0;	// Name: Path to the file relative to mFileDirectory
	//virtual HTexture VGetTexture(const char* const chName, (RECT?) coordinates) // Loades a smaller part of a texture
	virtual void	 VReleaseTexture(HTexture handle)=0;

	virtual U32 VGetWidth(HTexture handle) const=0;				// For all getter methods: return value always valid
	virtual U32 VGetHeight(HTexture handle) const=0;			// They return the stats of the default texture when 
	virtual DWORD VGetColorKey(HTexture handle) const=0;		// the handle points to nowhere. This should never happen
	virtual const std::string& VGetName(HTexture handle) const=0; 

protected:
	std::string mFileDirectory;
};


/*----------------------------------------------------------------*/
extern "C" {
	typedef HRESULT (*CREATERENDERINTERFACE)(HINSTANCE hDLL, const std::wstring &options, RIRenderInterface **pRI);
	typedef HRESULT (*RELEASERENDERINTERFACE)(RIRenderInterface **pRI);
}
/*----------------------------------------------------------------*/