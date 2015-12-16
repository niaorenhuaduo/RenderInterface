#pragma once

/********************************************************************/
// Desc:
// This file contains everything API dependent that the 
// implementation DLL has to implement
/********************************************************************/



/********************************************************************/
// INCLUDES
/********************************************************************/
#include "RI.h"
#include "HandleMgr.h"
#include "Color.h"
#include "Rectangle.h"



namespace ri 
{

/********************************************************************/
// GLOBALS / DEFINITIONS
/********************************************************************/
const U32   VIRTUALXRESOLUTION = 1280;
const U32   VIRTUALYRESOLUTION = 800;
const Color STANDARDCOLORKEY = Color(255, 255, 200, 200);


// Stencil settings
enum StencilTestType {
	RISTENCIL_DISABLED,
	RISTENCIL_WRITE,
	RISTENCIL_SCISSOR_WRITTEN,
	RISTENCIL_SCISSOR_EMPTY,
};


// For the Logfile
#ifndef _DEBUGFLUSH_
#define _DEBUGFLUSH_
#endif



/********************************************************************/
// FORWARD DECLARATIONS
/********************************************************************/
class Vec2;
class ITextureManager;
class Vertex2D;
class RenderState;


struct RenderSystemSettings {
	HWND		windowHandle;
	U32			windowWidth;
	U32			windowHeight;
	bool		bWindowed;

	bool		bTextureFilter; 
	bool		bMultiSampling;

	std::string mTextureFilesDirectory;
};


////////////////////////////////////////////////////////////
/// \brief The API independent rendersystem, every draw call goes through this
///
////////////////////////////////////////////////////////////
class RI_EXPORT IRenderSystem : private UnCopyable {												
public:
	IRenderSystem () {}		
	virtual ~IRenderSystem () {}

	////////////////////////////////////////////////////////////
	/// \brief Initialization, after this the interface can be used
	///
	/// \return true if the call succeeded
	/// 
	////////////////////////////////////////////////////////////
	virtual bool VOnInit(const RenderSystemSettings &settings)=0;

	////////////////////////////////////////////////////////////
	/// \brief Resize the window and switch to windowed/fullscreen
	///
	/// \return true if the call succeeded, when it fails nothing will change
	///
	////////////////////////////////////////////////////////////
	virtual bool VOnResize(U32 mWidth, U32 mHeight, bool bWindowed)=0;

	////////////////////////////////////////////////////////////
	/// \brief Begin the Render-Process
	///
	/// All draw calls have to follow up this method
	/// when it succeeds, if not: skip all render calls and VEndRendering() 
	///
	////////////////////////////////////////////////////////////
	virtual bool VBeginRendering()=0;

	////////////////////////////////////////////////////////////
	/// \brief Clears the desired buffers
	///
	////////////////////////////////////////////////////////////
	virtual void VClear(bool bRenderT, bool bDepth, bool bStencil)=0;

	////////////////////////////////////////////////////////////
	/// \brief Draws a textured and transformed quad on the screen
	///
	/// \param vertices		Pointer to an array of four vertices, define them clockwise and in local space
	/// \param renderstate	Desired renderstate
	///
	////////////////////////////////////////////////////////////
	virtual void VDrawPic(const Vertex2D *vertices, const RenderState &renderstate)=0;		

	////////////////////////////////////////////////////////////
	/// \brief Finish rendering by presenting the backbuffer to the screen, only call this when VBeginRendering succeeds
	///
	////////////////////////////////////////////////////////////
	virtual void VEndRendering()=0;


	// Set/Get
	void SetClearColor(const Color &color)	{ mClearColor = color; }
	void SetRunning(bool bRunning)			{ mbRunning = bRunning; }
	void VSetWindowed(bool bWindowed)		{ if(mbWindowed == bWindowed) return; VOnResize(mWindowWidth, mWindowHeight, bWindowed); }

	bool IsRunning() const			{ return mbRunning; }
	bool IsWindowed() const			{ return mbWindowed; }
	U32  GetWindowWidth() const		{ return mWindowWidth; }
	U32	 GetWindowHeight() const	{ return mWindowHeight; }


public:
	/********************************************************************/
	// Here is some low-level stuff defined that can be used for better performance
	// Only use this when necessary, instead use utility classes to manage all of this
	/********************************************************************/

	////////////////////////////////////////////////////////////
	/// \brief Sets the camera to the desired position
	///
	////////////////////////////////////////////////////////////
	virtual void _VSetCameraPosition(const Vec2 &centerPosition)=0;	

	////////////////////////////////////////////////////////////
	/// \brief Defines the size of the world that will get projected
	///
	/// \param width/height	Width/Height of the Camera in the world
	///
	////////////////////////////////////////////////////////////
	virtual void _VSetCameraSize(U32 width, U32 height)=0;	

	////////////////////////////////////////////////////////////
	/// \brief Describes on which part of the backbuffer the scene should be presented; width=[0, 1.0f]; height=[0, 1.0f]
	///
	////////////////////////////////////////////////////////////
	virtual void _VSetViewport(const FloatRect &viewPort)=0;

	////////////////////////////////////////////////////////////
	/// \brief This will flip the world upside down, this can be used to draw directly on the screen
	///
	////////////////////////////////////////////////////////////
	virtual void _VSetYFlipping(bool bFlipping)=0;


public:
	/********************************************************************/
	// Use this to load the desired implementation DLL and to create the desired render system
	/********************************************************************/

	////////////////////////////////////////////////////////////
	/// \brief Creates the desired implementation for the IRenderSystem and provides it 
	///		   as a service to the Service Locator
	///
	////////////////////////////////////////////////////////////
	static bool	CreateRenderSystem(const std::string &api);

	////////////////////////////////////////////////////////////
	/// \brief Releases the actual RenderSystem and removes it from the ServiceLocator
	///
	////////////////////////////////////////////////////////////
	static void	ReleaseRenderSystem();


protected:
	ITextureManager *mpTexManager;

	bool		mbRunning;
	U32			mWindowWidth;
	U32			mWindowHeight;
	bool		mbWindowed;
			
	Color		mClearColor;
	bool		mbTextureFilter;
	bool		mbMultiSampling;

	HWND		mhWnd;	// Window and DLL-Module
	HINSTANCE	mhDLL;
};




////////////////////////////////////////////////////////////
/// \brief Manages all texture resources
///
////////////////////////////////////////////////////////////
class RI_EXPORT ITextureManager : private UnCopyable {
public:

	////////////////////////////////////////////////////////////
	/// \brief The Texture Manager will load all textures from fileDirectory
	///
	////////////////////////////////////////////////////////////
	ITextureManager (const std::string &fileDirectory) { mFileDirectory = fileDirectory.c_str(); }

	////////////////////////////////////////////////////////////
	/// \brief Release all resources
	///
	////////////////////////////////////////////////////////////
	virtual ~ITextureManager () {}

	////////////////////////////////////////////////////////////
	/// \brief Returns a handle to the desired texture
	///
	/// It will only load the texture when it wasn't already loaded before
	///
	////////////////////////////////////////////////////////////
	virtual HTexture VGetTexture(const std::string &name)=0;	

	////////////////////////////////////////////////////////////
	/// \brief Releases the desired texture 
	///
	////////////////////////////////////////////////////////////
	virtual void VReleaseTexture(HTexture handle)=0;



	virtual U32 VGetWidth(HTexture handle) const=0;				// For all getter methods: return value always valid
	virtual U32 VGetHeight(HTexture handle) const=0;			// They return the stats of the default texture when 
	virtual U32 VGetColorKey(HTexture handle) const=0;			// the handle points to nowhere. This should never happen
	virtual const std::string& VGetName(HTexture handle) const=0; 

protected:
	const char *mFileDirectory;
};



} // namespace ri


/*----------------------------------------------------------------*/
extern "C" {
	typedef bool (*CREATERENDERSYSTEM)(HINSTANCE hDLL, ri::IRenderSystem **pRS);
	typedef bool (*RELEASERENDERSYSTEM)(ri::IRenderSystem **pRS);
}
/*----------------------------------------------------------------*/