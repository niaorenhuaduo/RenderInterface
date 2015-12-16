#include "..\Include\Image.h"


namespace ri 
{


Image::Image() : mWidth(0), mHeight(0), mData()
{}


void Image::Create( U32 width, U32 height, const Color &color ) {
	if(width == 0 || height == 0) return;

	mWidth  = width;
	mHeight = height;

	mData.clear();
	mData.assign(width * height, color);
}


void Image::Create( U32 width, U32 height, const Color *colorArray ) {
	if(width == 0 || height == 0 || colorArray == NULL) return;

	mWidth  = width;
	mHeight = height;

	U32 arrayLength = width * height;
	mData.resize(arrayLength);
	std::memcpy(&mData[0], colorArray, sizeof(Color) * arrayLength);
}


void Image::Resize( U32 width, U32 height ) {
	if(width == 0 || height == 0) return;

	// Temporary buffer for the old data
	std::vector<Color> temp(mData);

	// Resize the color array
	mData.resize(width * height);

	for(U32 x = 0; x < width; ++x) {
		for(U32 y = 0; y < height; ++y) {
			U32 index = y * width + x;

			// Additional space is black
			if(x >= mWidth || y >= mHeight) {
				mData[index] = Color(0, 0, 0);
				continue;
			}


			mData[index] = temp[y * mWidth + x];
		}
	}

	mWidth  = width;
	mHeight = height;
}

void Image::CopyFromImage( const Image &src, const IntRect &srcRect /*= IntRect(0, 0, 0, 0)*/, U32 dstPosX /*= 0*/, U32 dstPosY /*= 0*/ )  {
	if(src.GetWidth() == 0 || src.GetHeight() == 0 || mWidth == 0 || mHeight == 0 || dstPosX >= mWidth || dstPosY >= mHeight) return;

	// Find the source Rectangle
	IntRect tempRect = srcRect;
	if(tempRect.width == 0 || tempRect.height == 0) {
		tempRect.left	= 0;
		tempRect.top	= 0;
		tempRect.width	= src.GetWidth();
		tempRect.height = src.GetHeight();
	} else {
		if(tempRect.left < 0) tempRect.left = 0;
		if(tempRect.top  < 0) tempRect.top  = 0;
		if(tempRect.left + tempRect.width  > static_cast<I32>(src.GetWidth()))  tempRect.width  = src.GetWidth() - tempRect.left;
		if(tempRect.top  + tempRect.height > static_cast<I32>(src.GetHeight())) tempRect.height = src.GetHeight() - tempRect.top;
	}

	// Invalid srcRect coordinates
	if(tempRect.width <= 0 || tempRect.height <= 0) return;		


	// Find the destination rectangle
	U32 dstWidth  = tempRect.width;
	U32 dstHeight = tempRect.height;
	if(dstWidth  + dstPosX > mWidth)  dstWidth = mWidth - dstPosX;
	if(dstHeight + dstPosY > mHeight) dstHeight = mHeight - dstPosY;


	// Now copy the image
	Color*		 dstPointer = &mData[dstPosY * mWidth + dstPosX]; 
	const Color* srcPointer = src.GetPixelPtr() + (tempRect.top * src.GetWidth() + tempRect.left);
	U32			 pitch = dstWidth * sizeof(Color);

	for(U32 row = 0; row < dstHeight; ++row) {
		std::memcpy(dstPointer, srcPointer, pitch);
		dstPointer += mWidth;
		srcPointer += src.GetWidth();
	}
}

void Image::SetPixel( U32 xPos, U32 yPos, const Color &color ) {
	mData[yPos * mWidth + xPos] = color;
}

Color Image::GetPixel( U32 xPos, U32 yPos ) const {
	return mData[yPos * mWidth + xPos];
}


const Color* Image::GetPixelPtr() const {
	assert(!mData.empty() && "Image::GetPixelPtr()");

	return &mData[0];
}


} // namespace ri