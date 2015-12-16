#pragma once

/********************************************************************/
// INCLUDES
/********************************************************************/
#include "IRenderSystem.h"
#include "Matrix2D.h"




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
	/// > Default Texture
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
	/// > Default Texture
	/// > Alphablending is enabled
	///
	////////////////////////////////////////////////////////////
	RenderState(const Matrix2D &transformation);

	////////////////////////////////////////////////////////////
	/// \brief Default states with transparency set
	///
	/// > Default Texture
	/// > Identity Transformation
	///
	////////////////////////////////////////////////////////////
	RenderState(bool bAlpha);

	////////////////////////////////////////////////////////////
	/// \brief Fully customized RenderState
	///
	////////////////////////////////////////////////////////////
	RenderState(const Matrix2D &transformation, HTexture texture, bool bAlpha);


	// Data
	HTexture	  mTexture;
	Matrix2D	  mTransformation;
	bool		  mbAlpha;
};


// implementation
#include "RenderState.inl"


} // namespace ri