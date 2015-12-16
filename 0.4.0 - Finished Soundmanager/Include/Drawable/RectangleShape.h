#pragma once

#include "Drawable.h"
#include "..\Transformable.h"
#include "..\Color.h"
#include "..\Vec2.h"
#include "..\Vertex.h"
#include "..\Rectangle.h"


namespace ri {

////////////////////////////////////////////////////////////
/// \brief A transformable rectangle with a color
///
////////////////////////////////////////////////////////////
class RI_EXPORT RectangleShape : public Drawable, public Transformable {
public:

	////////////////////////////////////////////////////////////
	/// \brief Creates a 100x100 white square
	///
	////////////////////////////////////////////////////////////
	RectangleShape();

	////////////////////////////////////////////////////////////
	/// \brief Construct a rectangle, you can apply alpha with the color
	///
	/// \param width	Width of the rectangle  (0, infinite]
	/// \param height	Height of the rectangle (0, infinite]
	/// \param color	Fill color, you can apply alpha to the rectangle via the alpha channel
	///
	////////////////////////////////////////////////////////////
	RectangleShape(float width, float height, const Color &color = Color::White);

	////////////////////////////////////////////////////////////
	/// \brief Draws the transformed rectangle
	///
	////////////////////////////////////////////////////////////
	void OnDraw();

	////////////////////////////////////////////////////////////
	/// \brief Change the size of the rectangle
	///
	/// \param width	Width of the rectangle  (0, infinite]
	/// \param height	Height of the rectangle (0, infinite]
	///
	////////////////////////////////////////////////////////////
	void SetSize(float width, float height);
	void SetSize(const Vec2 &size);

	////////////////////////////////////////////////////////////
	/// \brief Change the color of the retangle
	///
	/// \param color	Fill color, you can apply alpha to the rectangle via the alpha channel
	///
	////////////////////////////////////////////////////////////
	void SetColor(const Color &color);



	////////////////////////////////////////////////////////////
	/// \brief Returns a bounding box that encloses the Rectangle in local space
	///
	////////////////////////////////////////////////////////////
	FloatRect			GetLocalBoundingBox() const;

	////////////////////////////////////////////////////////////
	/// \brief Returns an AABB that encloses the Rectangle after transformation
	///
	////////////////////////////////////////////////////////////
	FloatRect			GetGlobalBoundingBox() const; 

	const Vec2&  GetSize() const { return mSize; }
	const Color& GetColor() const { return mColor; }


private:

	Vec2		mSize;
	Color		mColor;
	Vertex2D	mVertices[6];
	FloatRect	mBoundingBox;


private:
	void RecalcGeometry();
};

} // namespace ri