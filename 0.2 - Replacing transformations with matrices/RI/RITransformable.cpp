#include "..\Include\RITransformable.h"

RITransformable::RITransformable() : 
	mPosition(0.0f, 0.0f),
	mOrigin(0.0f, 0.0f),
	mScaling(1.0f, 1.0f),
	mRotation(0.0f),
	mbTransformationNeedsUpdate(true),
	mTransformation(),
	mbInverseTransformationNeedsUpdate(true),
	mInverseTransformation()
{}

RITransformable::~RITransformable() {}

void RITransformable::SetPosition( float x, float y )
{
	mPosition.x = x;
	mPosition.y = y;

	mbTransformationNeedsUpdate = true;
	mbInverseTransformationNeedsUpdate = true;
}

void RITransformable::SetPosition( const RIVec2 &pos )
{
	SetPosition(pos.x, pos.y);
}

void RITransformable::AddToPosition( float x, float y )
{
	SetPosition(mPosition.x + x, mPosition.y + y);
}

void RITransformable::AddToPosition( const RIVec2 &offset )
{
	SetPosition(mPosition + offset);
}

void RITransformable::SetOrigin( float x, float y )
{
	mOrigin.x = x;
	mOrigin.y = y;

	mbTransformationNeedsUpdate = true;
	mbInverseTransformationNeedsUpdate = true;
}

void RITransformable::SetOrigin( const RIVec2 &origin )
{
	SetOrigin(origin.x, origin.y);
}

void RITransformable::SetScaling( float x, float y )
{
	mScaling.x = x;
	mScaling.y = y;

	mbTransformationNeedsUpdate = true;
	mbInverseTransformationNeedsUpdate = true;
}

void RITransformable::SetScaling( const RIVec2 &factors )
{
	SetScaling(factors.x, factors.y);
}

void RITransformable::SetScaling( float factor )
{
	SetScaling(factor, factor);
}

void RITransformable::Scale( float x, float y )
{
	SetScaling(mScaling.x * x, mScaling.y * y);
}

void RITransformable::Scale( const RIVec2 &factors )
{
	SetScaling(mScaling * factors);
}

void RITransformable::Scale( float factor )
{
	SetScaling(mScaling.x * factor, mScaling.y * factor);
}

void RITransformable::SetRotation( float degrees )
{
	mRotation = degrees;

	mbTransformationNeedsUpdate = true;
	mbInverseTransformationNeedsUpdate = true;
}

void RITransformable::Rotate( float degrees )
{
	SetRotation(mRotation + degrees);
}


const RIMatrix2D& RITransformable::GetTransformation() const
{
	if(mbTransformationNeedsUpdate) {
		mTransformation.Identity();

		mTransformation.Translate(-mOrigin);
		mTransformation.Rotate(mRotation);
		mTransformation.Scale(mScaling);
		mTransformation.Translate(mPosition);

		mbTransformationNeedsUpdate = false;
	}

	return mTransformation;
}

const RIMatrix2D& RITransformable::GetInverseTransformation() const
{
	if(mbInverseTransformationNeedsUpdate) {
		mInverseTransformation = GetTransformation().GetInverse();

		mbInverseTransformationNeedsUpdate = false;
	}

	return mInverseTransformation;
}
