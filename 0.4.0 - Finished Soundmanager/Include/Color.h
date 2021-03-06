#pragma once

/********************************************************************/
// INCLUDES
/********************************************************************/
#include "RI.h"


#pragma warning( disable : 4201 )


namespace ri 
{

////////////////////////////////////////////////////////////
/// \brief Utility class for a color, 32bit
///
////////////////////////////////////////////////////////////
class RI_EXPORT Color {
public:
	/********************************************************************/
	// Useful colors
	/********************************************************************/
	static const Color White;
	static const Color Black;

public:

	////////////////////////////////////////////////////////////
	/// \brief Black color, (0, 0, 0, 255)
	///
	////////////////////////////////////////////////////////////
	Color();

	////////////////////////////////////////////////////////////
	/// \brief Every channel in the range [0, 255]
	///
	////////////////////////////////////////////////////////////
	Color(U8 red, U8 green, U8 blue, U8 alpha = 255);


	operator U32() const { return color; }

public:

	// [32] aaaaaaaa rrrrrrrr gggggggg bbbbbbbb [0]
	union {
		struct {
			U8 b;
			U8 g;
			U8 r;
			U8 a;
		};
		U32 color;
	};
};


/********************************************************************/
// TODO: operators: ==, !=, *= and so on
/********************************************************************/


// implementation
#include "Color.inl"

} // namespace ri



