#pragma once

/********************************************************************/
// INCLUDES
/********************************************************************/
#include "RI.h"



namespace ri 
{

////////////////////////////////////////////////////////////
/// \brief Utility class for rectangles, I32
///
////////////////////////////////////////////////////////////
class RI_EXPORT IntRect {
public:
	IntRect();
	IntRect(I32 _left, I32 _top, I32 _width, I32 _height);

	I32 left;
	I32 top;
	I32 width;
	I32 height;
};


////////////////////////////////////////////////////////////
/// \brief Utility class for rectangles, float
///
////////////////////////////////////////////////////////////
class RI_EXPORT FloatRect {
public:
	FloatRect();
	FloatRect(float _left, float _top, float _width, float _height);

	float left;
	float top;
	float width;
	float height;
};




// implementation 
#include "Rectangle.inl"



} // namespace ri