#pragma once

/********************************************************************/
// INCLUDE
/********************************************************************/
#include "RI.h"
#include "Color.h"
#include "Rectangle.h"
#include <vector>


// TODO: Replace the vector with something else
#pragma warning( disable : 4251 )


namespace ri 
{

////////////////////////////////////////////////////////////
/// \brief Represents a editable texture in system memory. It can be used to load a real texture.
///
////////////////////////////////////////////////////////////
class RI_EXPORT Image {
public:

	////////////////////////////////////////////////////////////
	/// \brief Empty Image
	///
	////////////////////////////////////////////////////////////
	Image();

	////////////////////////////////////////////////////////////
	/// \brief Creates an image with a specified color and size
	///
	////////////////////////////////////////////////////////////
	void Create(U32 width, U32 height, const Color &color);

	////////////////////////////////////////////////////////////
	/// \brief Creates an image and loads it from an Color array
	///
	/// The colorArray has to be bigger or equal to [width*height].
	/// Everything else will result in undefined behavior!
	///
	/// \param width	  Width of the image
	/// \param height	  Height of the image
	/// \param colorArray A pointer to an array of Colors. The array has to have a size of [width * height]!
	///
	////////////////////////////////////////////////////////////
	void Create(U32 width, U32 height, const Color *colorArray);

	////////////////////////////////////////////////////////////
	/// \brief This will resize the image to the specified size.
	///
	/// Supernatant space will be cut off and additional space will be filled with black
	///
	////////////////////////////////////////////////////////////
	void Resize(U32 width, U32 height, const Color &fillColor = Color::White);

	////////////////////////////////////////////////////////////
	/// \brief Copy a subrectangle of src to this Image at the specified position. Supernatant space will be cut off.
	///
	////////////////////////////////////////////////////////////
	void CopyFromImage(const Image &src, const IntRect &srcRect = IntRect(0, 0, 0, 0), U32 destPosX = 0, U32 destPosY = 0);

	////////////////////////////////////////////////////////////
	/// \brief Set the pixel at the specified position to color. 
	///
	/// Out of range coordinates will result in undefined behavior
	///
	////////////////////////////////////////////////////////////
	void SetPixel(U32 xPos, U32 yPos, const Color &color);

	////////////////////////////////////////////////////////////
	/// \brief Get the color of the pixel at the specified position.
	///
	/// Out of range coordinates will result in undefined behavior
	///
	////////////////////////////////////////////////////////////
	Color GetPixel(U32 xPos, U32 yPos) const;


	////////////////////////////////////////////////////////////
	/// \brief Save the image to a bmp file, existing files will be overriden
	///
	////////////////////////////////////////////////////////////
	bool SaveToFile(const std::string &fileName) const;


	U32			 GetWidth() const    { return mWidth; }
	U32			 GetHeight() const   { return mHeight; }
	const Color* GetPixelPtr() const;


private:
	U32 mWidth;
	U32 mHeight; 
	std::vector<Color> mData;
};


} // namespace ri