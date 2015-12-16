#pragma once

/********************************************************************/
// INCLUDES
/********************************************************************/
#include "Font.h"
#include "Transformable.h"
#include "Color.h"
#include "Rectangle.h"
#include "Vertex.h"
#include <string>
#include <vector>


namespace ri {


////////////////////////////////////////////////////////////
/// \brief A transformable text than can be drawn on the screen
///
////////////////////////////////////////////////////////////
class Label : public Transformable {
public:

	////////////////////////////////////////////////////////////
	/// \brief Empty label
	///
	////////////////////////////////////////////////////////////
	Label();

	////////////////////////////////////////////////////////////
	/// \brief Label with a specific font and size
	///
	////////////////////////////////////////////////////////////
	Label(const std::string &text, const Font &font, U32 textSize);

	////////////////////////////////////////////////////////////
	/// \brief Draws the transformed label on the screen
	///
	////////////////////////////////////////////////////////////
	void OnDraw();





private:
	std::string		mText;
	const Font	   *mpFont;
	U32				mTextSize;
	Color			mColor;
	
	FloatRect			  mBoundingBox;
	std::vector<Vertex2D> mVertices;
	bool				  mbGeometryNeedsUpdate;

	bool mbItalic;
	bool mbUnderlined;
	bool mbBold;

private:
	void RecalcGeometry();
};

} // namespace ri