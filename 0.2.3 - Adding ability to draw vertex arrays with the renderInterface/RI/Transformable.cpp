#include "..\Include\Transformable.h"


namespace ri 
{


Transformable::Transformable() : 
	mPosition(0.0f, 0.0f),
	mOrigin(0.0f, 0.0f),
	mScaling(1.0f, 1.0f),
	mRotation(0.0f),
	mbTransformationNeedsUpdate(true),
	mTransformation(),
	mbInverseTransformationNeedsUpdate(true),
	mInverseTransformation()
{}

Transformable::~Transformable() {}

void Transformable::SetPosition( float x, float y )
{
	mPosition.x = x;
	mPosition.y = y;

	mbTransformationNeedsUpdate = true;
	mbInverseTransformationNeedsUpdate = true;
}

void Transformable::SetPosition( const Vec2 &pos )
{
	SetPosition(pos.x, pos.y);
}

void Transformable::AddToPosition( float x, float y )
{
	SetPosition(mPosition.x + x, mPosition.y + y);
}

void Transformable::AddToPosition( const Vec2 &offset )
{
	SetPosition(mPosition + offset);
}

void Transformable::SetOrigin( float x, float y )
{
	mOrigin.x = x;
	mOrigin.y = y;

	mbTransformationNeedsUpdate = true;
	mbInverseTransformationNeedsUpdate = true;
}

void Transformable::SetOrigin( const Vec2 &origin )
{
	SetOrigin(origin.x, origin.y);
}

void Transformable::SetScaling( float x, float y )
{
	mScaling.x = x;
	mScaling.y = y;

	mbTransformationNeedsUpdate = true;
	mbInverseTransformationNeedsUpdate = true;
}

void Transformable::SetScaling( const Vec2 &factors )
{
	SetScaling(factors.x, factors.y);
}

void Transformable::SetScaling( float factor )
{
	SetScaling(factor, factor);
}

void Transformable::Scale( float x, float y )
{
	SetScaling(mScaling.x * x, mScaling.y * y);
}

void Transformable::Scale( const Vec2 &factors )
{
	SetScaling(mScaling * factors);
}

void Transformable::Scale( float factor )
{
	SetScaling(mScaling.x * factor, mScaling.y * factor);
}

void Transformable::SetRotation( float degrees )
{
	mRotation = degrees;

	mbTransformationNeedsUpdate = true;
	mbInverseTransformationNeedsUpdate = true;
}

void Transformable::Rotate( float degrees )
{
	SetRotation(mRotation + degrees);
}


const Matrix2D& Transformable::GetTransformation() const
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

const Matrix2D& Transformable::GetInverseTransformation() const
{
	if(mbInverseTransformationNeedsUpdate) {
		mInverseTransformation = GetTransformation().GetInverse();

		mbInverseTransformationNeedsUpdate = false;
	}

	return mInverseTransformation;
}



} // namespace ri