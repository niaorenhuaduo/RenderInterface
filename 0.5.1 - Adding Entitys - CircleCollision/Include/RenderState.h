#pragma once

/********************************************************************/
// INCLUDES
/********************************************************************/
#include "Matrix2D.h"
#include "HandleMgr.h"



namespace ri
{

////////////////////////////////////////////////////////////
/// \brief Renderstates to use while rendering
///
////////////////////////////////////////////////////////////
class RI_EXPORT RenderState {
public:

	////////////////////////////////////////////////////////////
	/// \brief Default RenderState
	///
	/// > No Texture
	/// > Identity Transformation
	/// > Alphablending is enabled
	///
	////////////////////////////////////////////////////////////
	RenderState();

	////////////////////////////////////////////////////////////
	/// \brief Default states with a texture
	///
	/// > Identity Transformation
	/// > Alphablending is enabled
	///
	////////////////////////////////////////////////////////////
	RenderState(HTexture texture);

	////////////////////////////////////////////////////////////
	/// \brief Default states with a transformation
	///
	/// > No Texture
	/// > Alphablending is enabled
	///
	////////////////////////////////////////////////////////////
	RenderState(const Matrix2D &transformation);

	////////////////////////////////////////////////////////////
	/// \brief Default states with transparency set
	///
	/// > No Texture
	/// > Identity Transformation
	///
	////////////////////////////////////////////////////////////
	RenderState(bool bAlpha);

	////////////////////////////////////////////////////////////
	/// \brief Fully customized RenderState
	///
	////////////////////////////////////////////////////////////
	RenderState(const Matrix2D &transformation, HTexture texture, bool bAlpha);


	// The same as the default constructor
	static const RenderState Default; 

	// Data
	HTexture	  mTexture;
	Matrix2D	  mTransformation;
	bool		  mbAlpha;
};


// implementation
#include "RenderState.inl"


} // namespace ri