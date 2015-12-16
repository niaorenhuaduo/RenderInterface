#include "..\Include\Font.h"

namespace ri {


Font::Font() {

}

Font::~Font() {

}

const Glyph& Font::GetGlyph( U32 unicode, U32 fontSize, bool bold ) const {
	return Glyph();
}

ri::HTexture Font::GetTexture( U32 fontSize ) const {
	return HTexture();
}

float Font::GetYOffset( U32 fontSize ) const {
	return 0.0f;
}

} // namespace ri