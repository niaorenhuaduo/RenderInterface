#pragma once

#include "..\RI.h"


namespace ri {

////////////////////////////////////////////////////////////
/// \brief A base class for all renderable objects
///
////////////////////////////////////////////////////////////
class RI_EXPORT Drawable {
public:
	virtual ~Drawable() {}

	virtual void OnDraw()=0;
};

} // namespace ri