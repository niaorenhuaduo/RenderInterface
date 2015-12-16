#pragma once

#include "RI.h"

///////////////////////////////////////////////////////////
// TODO
// Optimize GetTransformation
///////////////////////////////////////////////////////////


class RITransformable {
public:
	RITransformable();
	virtual ~RITransformable();

	void SetPosition(float x, float y);
	void SetPosition(const RIVec2 &pos);
	void AddToPosition(float x, float y);
	void AddToPosition(const RIVec2 &offset);

	void SetOrigin(float x, float y);
	void SetOrigin(const RIVec2 &origin);

	void SetScaling(float x, float y);
	void SetScaling(const RIVec2 &factors);
	void SetScaling(float factor);
	void Scale(float x, float y);				// multiplies with existing scaling
	void Scale(const RIVec2 &factors);			// multiplies with existing scaling
	void Scale(float factor);					// multiplies with existing scaling

	void SetRotation(float degrees);			// Sets rotation
	void Rotate(float degrees);					// Adds rotation 

	// Getter
	const RIVec2& GetPosition() const	{ return mPosition; }
	const RIVec2& GetOrigin() const		{ return mOrigin; }
	const RIVec2& GetScaling() const	{ return mScaling; }
	float		  GetRotation() const	{ return mRotation; }

	const RIMatrix2D& GetTransformation() const;
	const RIMatrix2D& GetInverseTransformation() const;

private:
	RIVec2 mPosition;				// Position of the Origin
	RIVec2 mOrigin;					// scale and rotate around this point; relative to the upper left corner
	RIVec2 mScaling;
	float  mRotation;				// in degrees
		
	// They are mutable to ensure GetTransformation can remain const
	mutable bool		mbTransformationNeedsUpdate;
	mutable RIMatrix2D	mTransformation;
	mutable bool		mbInverseTransformationNeedsUpdate;
	mutable RIMatrix2D	mInverseTransformation;
};