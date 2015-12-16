#pragma once

/********************************************************************/
// INCLUDES
/********************************************************************/
#include "..\Font.h"
#include "..\Transformable.h"
#include "..\Color.h"
#include "..\Rectangle.h"
#include "..\Vertex.h"
#include "Drawable.h"
#include <string>
#include <vector>


namespace ri {


////////////////////////////////////////////////////////////
/// \brief A transformable text than can be drawn on the screen
///
////////////////////////////////////////////////////////////
class RI_EXPORT Label : public Drawable, public Transformable {
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



	////////////////////////////////////////////////////////////
	/// \brief Sets the text that will be presented
	///
	////////////////////////////////////////////////////////////
	void SetText(const std::string &text);

	////////////////////////////////////////////////////////////
	/// \brief Change the actual font
	///
	////////////////////////////////////////////////////////////
	void SetFont(const Font &newFont);

	////////////////////////////////////////////////////////////
	/// \brief Sets the text size
	///
	////////////////////////////////////////////////////////////
	void SetTextSize(U32 textSize);

	////////////////////////////////////////////////////////////
	/// \brief Sets the color of the label, alpha values are not supported
	///
	////////////////////////////////////////////////////////////
	void SetColor(const Color &color);

	////////////////////////////////////////////////////////////
	/// \brief Enable/Disable shadows for this label
	///
	/// \param shadowColor  Color of the shadow
	/// \param shadowOffset x/y-Offset to the text, leave it at 0.0f for standard shadow Offset
	///
	////////////////////////////////////////////////////////////
	void SetShadowed(bool bShadowed, const Color &shadowColor = Color::Black, const Vec2 &shadowOffset = Vec2(0.0f));

	void SetBold(bool bBold);
	void SetUnderlined(bool bUnderlined);


	const Font*			GetFont() const		{ return mpFont; }
	U32					GetTextSize() const { return mTextSize; }
	const std::string&	GetText() const		{ return mText; }
	const Color&		GetColor() const	{ return mColor; }
	bool				IsBold() const		{ return mbBold; }
	bool				IsUnderlined() const{ return mbUnderlined; }

	////////////////////////////////////////////////////////////
	/// \brief Returns a bounding box that encloses the Label in local space
	///
	////////////////////////////////////////////////////////////
	FloatRect			GetLocalBoundingBox() const;

	////////////////////////////////////////////////////////////
	/// \brief Returns an AABB that encloses the Label after transformation
	///
	////////////////////////////////////////////////////////////
	FloatRect			GetGlobalBoundingBox() const;


private:
	std::string		mText;
	const Font	   *mpFont;
	U32				mTextSize;
	Color			mColor;
	
	mutable FloatRect			  mBoundingBox;
	mutable std::vector<Vertex2D> mVertices;
	mutable bool				  mbGeometryNeedsUpdate;
	mutable U32					  mActTextureSize;		// used to prevent display bugs after the font texture got resized

	bool mbUnderlined;
	bool mbBold;

	bool  mbShadowed;
	Color mShadowColor;
	Vec2  mShadowOffset;

private:
	void RecalcGeometry() const;
};

} // namespace ri