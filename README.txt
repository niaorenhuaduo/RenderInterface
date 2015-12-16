Engine Design

<< 3rdParty >>
Contains all projects that the main engine depends on

Folder 3RDPARTYLIBRARIES: Contains all the required libraries for the engine to run properly.


<< RI >>
A DLL which offers classes to draw 2D stuff on the screen. Every Drawable class is using the
abstract IRenderSystem to draw their stuff. When initializing RI you have to specify which API
you want to use, the engine will automatically load the analogue DLL.
 
*Dependencies:
 -Include: 		*3rdParty\boost_1_55_0b1
			*3rdParty\freetype\Include
			*3rdParty\visualleakdetector\include		(Only on Debug)
				
 -Lib:			*3rdParty\freetype\Lib
				-freetype6.dll
				-zlib1.dll
			*3rdParty\visualleakdetector\lib\Win32		(Only on Debug)
				-vld_x86.dll
				-dbghelp.dll
				-Microsoft.DTfW.DHL.manifest
 
 
 
*Classes:
			*IRenderSystem: This is the core of the engine. All render calls go through this abstract 
					class. It is API-Independant and I tried to make it as simple as possible.
					For this engine to work, you need an additional dynamic library that 
					implements all of IRenderSystem and ITextureManager.

 -Drawable:		*Drawable:	Abstract Parent class for all drawable objects
			*Sprite
			*Entity
			*RectangleShape
			*Label
			
 -Features:		*Camera
			*Font
			*Image
			*Transformable
			
 -Helper: 		// Math
			*Color
			*Vec2
			*Vec4
			*Matrix2D
			*Rectangle
			*RIMath
			*Vertex
				
			// other
			*DynLibrary
			*HandleMgr
			*ServiceLocator
				
 
 
 << RSD9 >>
 A DLL that implements IRenderSystem and ITextureManager using DirectX 9
 
 *Dependencies:
 	-Include: 	*(DXSDK_DIR)Include
			*3rdParty\boost_1_55_0b1
			*3rdParty\visualleakdetector\include		(Only on Debug)
			*RI\Include
				
	-Lib:		*(DXSDK_DIR)Lib\x86
			*3rdParty\visualleakdetector\lib\Win32		(Only on Debug)
			*RI\Lib
				
	Links:		*RI.lib
			*d3d9.lib
			*d3dx9.lib
			*dxerr.lib
 