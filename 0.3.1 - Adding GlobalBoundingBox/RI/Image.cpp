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


void Image::Resize( U32 width, U32 height, const Color &fillColor ) {
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
				mData[index] = fillColor;
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


namespace {
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>


	typedef unsigned int stbiw_uint32;
	typedef int stb_image_write_test[sizeof(stbiw_uint32)==4 ? 1 : -1];

	static void writefv(FILE *f, const char *fmt, va_list v)
	{
		while (*fmt) {
			switch (*fmt++) {
			case ' ': break;
			case '1': { unsigned char x = (unsigned char) va_arg(v, int); fputc(x,f); break; }
			case '2': { int x = va_arg(v,int); unsigned char b[2];
				b[0] = (unsigned char) x; b[1] = (unsigned char) (x>>8);
				fwrite(b,2,1,f); break; }
			case '4': { stbiw_uint32 x = va_arg(v,int); unsigned char b[4];
				b[0]=(unsigned char)x; b[1]=(unsigned char)(x>>8);
				b[2]=(unsigned char)(x>>16); b[3]=(unsigned char)(x>>24);
				fwrite(b,4,1,f); break; }
			default:
				assert(0);
				return;
			}
		}
	}

	static void write3(FILE *f, unsigned char a, unsigned char b, unsigned char c)
	{
		unsigned char arr[3];
		arr[0] = a, arr[1] = b, arr[2] = c;
		fwrite(arr, 3, 1, f);
	}

	static void write_pixels(FILE *f, int rgb_dir, int vdir, int x, int y, int comp, void *data, int write_alpha, int scanline_pad)
	{
		unsigned char bg[3] = { 255, 0, 255}, px[3];
		stbiw_uint32 zero = 0;
		int i,j,k, j_end;

		if (y <= 0)
			return;

		if (vdir < 0) 
			j_end = -1, j = y-1;
		else
			j_end =  y, j = 0;

		for (; j != j_end; j += vdir) {
			for (i=0; i < x; ++i) {
				unsigned char *d = (unsigned char *) data + (j*x+i)*comp;
				if (write_alpha < 0)
					fwrite(&d[comp-1], 1, 1, f);
				switch (comp) {
				case 1:
				case 2: write3(f, d[0],d[0],d[0]);
					break;
				case 4:
					if (!write_alpha) {
						// composite against pink background
						for (k=0; k < 3; ++k)
							px[k] = bg[k] + ((d[k] - bg[k]) * d[3])/255;
						write3(f, px[1-rgb_dir],px[1],px[1+rgb_dir]);
						break;
					}
					/* FALLTHROUGH */
				case 3:
					write3(f, d[1-rgb_dir],d[1],d[1+rgb_dir]);
					break;
				}
				if (write_alpha > 0)
					fwrite(&d[comp-1], 1, 1, f);
			}
			fwrite(&zero,scanline_pad,1,f);
		}
	}

	static int outfile(char const *filename, int rgb_dir, int vdir, int x, int y, int comp, void *data, int alpha, int pad, const char *fmt, ...)
	{
		FILE *f;
		if (y < 0 || x < 0) return 0;
		f = fopen(filename, "wb");
		if (f) {
			va_list v;
			va_start(v, fmt);
			writefv(f, fmt, v);
			va_end(v);
			write_pixels(f,rgb_dir,vdir,x,y,comp,data,alpha,pad);
			fclose(f);
		}
		return f != NULL;
	}
}



bool Image::SaveToFile( const std::string &fileName ) const
{
	int pad = (-mWidth*3) & 3;
	return outfile(fileName.c_str(),-1,-1, mWidth, mHeight, 4,(void *) &mData[0],0,pad,
		"11 4 22 4" "4 44 22 444444",
		'B', 'M', 14+40+(mWidth*3+pad)*mHeight, 0,0, 14+40,  // file header
		40, mWidth,mHeight, 1,24, 0,0,0,0,0,0);             // bitmap header
}


} // namespace ri