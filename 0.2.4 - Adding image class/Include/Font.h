#pragma once

/********************************************************************/
// INCLUDES
/********************************************************************/
#include "HandleMgr.h"
#include "Rectangle.h"



namespace ri {


////////////////////////////////////////////////////////////
/// \brief Represents a character. Used to draw text
///
////////////////////////////////////////////////////////////
class Glyph {
public:
	Glyph() : AdvanceX(0), TexCoords(), BoundingBox() {}
	
	IntRect		TexCoords;		// texture coordinates in the font texture
	FloatRect	BoundingBox;	// Rectangle of the glyph relative to the base line 
	float		AdvanceX;		// Advance by this offset after rendering this glyph
};


////////////////////////////////////////////////////////////
/// \brief A font that can be loaded from file. Non-Bitmap fonts support font scaling
///
////////////////////////////////////////////////////////////
class Font {
public:

	Font();
	~Font();

	const Glyph& GetGlyph(U32 unicode, U32 fontSize, bool bold) const;

	HTexture GetTexture(U32 fontSize) const;

	float GetYOffset(U32 fontSize) const; 

private:

};


} // namespace ri