#include "..\Include\Drawable\Entity.h"


namespace ri {



Entity::Entity() : ri::Sprite()
{
	Initialize();
}

Entity::Entity( ri::HTexture tex, bool bAlpha /*= false*/ ) : ri::Sprite(tex, bAlpha)
{
	Initialize();
}

Entity::Entity( ri::HTexture tex, const ri::IntRect &textureRect, bool bAlpha /*= false*/ ) : ri::Sprite(tex, textureRect, bAlpha)
{
	Initialize();
}

Entity::Entity( ri::HTexture tex, U32 frameWidth, U32 frameHeight, U32 numColumns, U32 startFrame, U32 endFrame, double frameDelay, bool bLooped /*= false*/, bool bAlpha /*= false*/ )
	: ri::Sprite(tex, frameWidth, frameHeight, numColumns, startFrame, endFrame, frameDelay, bLooped, bAlpha)
{
	Initialize();
}


void Entity::Initialize()
{
	mType			= CT_CIRCLE;
	mfRadius		= 1.0f;

	mfMass			= 1.0f;
	mVelocity		= ri::Vec2();
	mbActive		= true;

	mEdge.top		= -1;
	mEdge.bottom	=  1;
	mEdge.left		= -1;
	mEdge.right		=  1;

	mbRotatedBoxReady = false;
}

void Entity::OnUpdate( double fElapsedTime )
{
	mVelocity += mDeltaVelocity;
	mDeltaVelocity.x = mDeltaVelocity.y = 0.0f;

	AddToPosition(mVelocity * static_cast<float>(fElapsedTime));

	ri::Sprite::OnUpdate(fElapsedTime);

	mbRotatedBoxReady = false;
}

bool Entity::CollidesWith( const Entity &ent, ri::Vec2 *collisionVector )
{
	if(!GetActive() || !ent.GetActive()) return false;

	if     (GetCollisionType() == CT_CIRCLE && ent.GetCollisionType() == CT_CIRCLE) return EntityCollideCircle(*this, ent, collisionVector);
	else if(GetCollisionType() == CT_AABB   && ent.GetCollisionType() == CT_AABB)   return EntityCollideAABB(*this, ent, collisionVector);
	else if(GetCollisionType() != CT_CIRCLE && ent.GetCollisionType() != CT_CIRCLE) {  // AABB/OOBB  OR  OOBB/AABB  OR  OOBB/OOBB
		return EntityCollideOOBB(*this, ent, collisionVector);
	} else {  // one of the entities is a circle, the other AABB/OOBB
		if(GetCollisionType() == CT_CIRCLE) return EntityCollideBoxCircle(ent, *this, collisionVector);
		else								return EntityCollideBoxCircle(*this, ent, collisionVector);
	}
}


void Entity::Bounce( const ri::Vec2 &collisionVector, const Entity &ent )
{
	ri::Vec2 vdiff = ent.GetVelocity() - GetVelocity();
	ri::Vec2 cUV = collisionVector;
	cUV.Normalize();

	float cUVdotVdiff = vdiff.Dot(cUV);
	float massRatio = 2.0f;
	if(GetMass() != 0.0f) massRatio *= (ent.GetMass() / (GetMass() + ent.GetMass()));

	if(cUVdotVdiff > 0) {
		AddToPosition(-cUV.x * massRatio, -cUV.y * massRatio);
	} else 
		mDeltaVelocity += (cUV * (massRatio * cUVdotVdiff));
}




void Entity::ComputeRotatedBox() const
{
	if(mbRotatedBoxReady) return;

	ri::Vec2 center		= GetPosition();
	float	 fRotation	= GetRotation();
	float	 fScaling	= GetScaling();

	ri::Vec2 rotatedX = ri::Vec2(cos(fRotation),  sinf(fRotation));
	ri::Vec2 rotatedY = ri::Vec2(-sin(fRotation), cosf(fRotation));

	//	0-------1
	//	|		|
	//	|		|
	//	3-------2

	mCorners[0] = center +	rotatedX * static_cast<float>(mEdge.left	* fScaling) +
		rotatedY * static_cast<float>(mEdge.top		* fScaling);
	mCorners[1] = center +	rotatedX * static_cast<float>(mEdge.right	* fScaling) +
		rotatedY * static_cast<float>(mEdge.top		* fScaling);
	mCorners[2] = center +	rotatedX * static_cast<float>(mEdge.right	* fScaling) +
		rotatedY * static_cast<float>(mEdge.bottom	* fScaling);
	mCorners[3] = center +	rotatedX * static_cast<float>(mEdge.left	* fScaling) +
		rotatedY * static_cast<float>(mEdge.bottom	* fScaling);

	// calculate the edges
	mEdge01 = mCorners[1] - mCorners[0];
	mEdge01.Normalize();

	mEdge03 = mCorners[3] - mCorners[0];
	mEdge03.Normalize();

	// Projection on edge 01
	float projection = mEdge01.Dot(mCorners[0]);
	mEdge01Min = projection;
	mEdge01Max = projection;

	projection = mEdge01.Dot(mCorners[1]);
	if(projection < mEdge01Min)			mEdge01Min = projection;
	else if(projection > mEdge01Max)	mEdge01Max = projection;

	// projection on edge 03
	projection = mEdge03.Dot(mCorners[0]);
	mEdge03Min = projection;
	mEdge03Max = projection;

	projection = mEdge03.Dot(mCorners[3]);
	if(projection < mEdge03Min)			mEdge03Min = projection;
	else if(projection > mEdge03Max)	mEdge03Max = projection;

	mbRotatedBoxReady = true;
}

bool Entity::ProjectionsOverlap( const Entity &ent ) const
{
	float projection, min01, max01, min03, max03;

	// project other box onto edge01
	projection = mEdge01.Dot(ent.GetCorner(0));
	min01 = projection;
	max01 = projection;

	for(int c=1; c<4; ++c) {
		projection = mEdge01.Dot(ent.GetCorner(c));
		if(projection < min01)		min01 = projection;
		else if(projection > max01) max01 = projection;
	}

	if(min01 > mEdge01Max || max01 < mEdge01Min) return false;

	// project onto edge03
	projection = mEdge03.Dot(ent.GetCorner(0));
	min03 = projection;
	max03 = projection;

	for(int c=1; c<4; ++c) {
		projection = mEdge03.Dot(ent.GetCorner(c));
		if(projection < min03)		min03 = projection;
		else if(projection > max03) max03 = projection;
	}

	if(min03 > mEdge03Max || max03 < mEdge03Min) return false;

	return true;
}









bool EntityCollideCircle( const Entity &ent1, const Entity &ent2, ri::Vec2 *collisionVector )
{
	ri::Vec2 distSquared = ent1.GetPosition() - ent2.GetPosition();
	distSquared.x = distSquared.x * distSquared.x;
	distSquared.y = distSquared.y * distSquared.y;

	float sumRadiusSquared = (ent1.GetRadius() * ent1.GetScaling()) + (ent2.GetRadius() * ent2.GetScaling());
	sumRadiusSquared *= sumRadiusSquared;

	if((distSquared.x + distSquared.y) <= sumRadiusSquared) { // collision
		if(collisionVector) *collisionVector = ent1.GetPosition() - ent2.GetPosition();

		return true;
	} 

	return false;
}

bool EntityCollideAABB( const Entity &ent1, const Entity &ent2, ri::Vec2 *collisionVector )
{
	ri::FloatRect A = ent1.GetGlobalBoundingBox();
	ri::FloatRect B = ent2.GetGlobalBoundingBox();

	if((A.left + A.width	>= B.left) &&
		(A.left				<= B.left + B.width) &&
		(A.top				<= B.top  + B.height) &&
		(A.top  + A.height   >= B.top)) 
	{ // collision
		if(collisionVector) *collisionVector = ent1.GetPosition() - ent2.GetPosition();

		return true;
	}

	return false;
}

bool EntityCollideOOBB( const Entity &ent1, const Entity &ent2, ri::Vec2 *collisionVector )
{
	ent1.ComputeRotatedBox();
	ent2.ComputeRotatedBox();

	if(ent1.ProjectionsOverlap(ent2) && ent2.ProjectionsOverlap(ent1)) {
		if(collisionVector) *collisionVector = ent1.GetPosition() - ent2.GetPosition();

		return true;
	}

	return false;
}

bool EntityCollideBoxCircle( const Entity &ent1, const Entity &ent2, ri::Vec2 *collisionVector )
{
	float min01, max01, min03, max03, center01, center03;
	ent1.ComputeRotatedBox();

	center01 = ent1.mEdge01.Dot(ent2.GetPosition());
	min01 = center01 - (ent2.GetRadius() * ent2.GetScaling());
	max01 = center01 + (ent2.GetRadius() * ent2.GetScaling());
	if(min01 > ent1.mEdge01Max || max01 < ent1.mEdge01Min) return false;


	center03 = ent1.mEdge03.Dot(ent2.GetPosition());
	min03 = center03 - (ent2.GetRadius() * ent2.GetScaling());
	max03 = center03 + (ent2.GetRadius() * ent2.GetScaling());
	if(min03 > ent1.mEdge03Max || max03 < ent1.mEdge03Min) return false;

	// check if circle is in voronoi region
	if(center01 < ent1.mEdge01Min && center03 < ent1.mEdge03Min) 
		return PointCollidesCircle(ent1.GetCorner(0), ent2.GetPosition(), ent2.GetRadius() * ent2.GetScaling(), collisionVector);
	if(center01 < ent1.mEdge01Max && center03 < ent1.mEdge03Min) 
		return PointCollidesCircle(ent1.GetCorner(1), ent2.GetPosition(), ent2.GetRadius() * ent2.GetScaling(), collisionVector);
	if(center01 < ent1.mEdge01Max && center03 < ent1.mEdge03Max) 
		return PointCollidesCircle(ent1.GetCorner(2), ent2.GetPosition(), ent2.GetRadius() * ent2.GetScaling(), collisionVector);
	if(center01 < ent1.mEdge01Min && center03 < ent1.mEdge03Max) 
		return PointCollidesCircle(ent1.GetCorner(3), ent2.GetPosition(), ent2.GetRadius() * ent2.GetScaling(), collisionVector);

	// circle not in voronoi region, so its colliding with edge of box
	if(collisionVector) *collisionVector = ent1.GetPosition() - ent2.GetPosition();
	return true;
}

bool PointCollidesCircle( const ri::Vec2 &point, const ri::Vec2 &circleCenter, float circleRadius, ri::Vec2 *collisionVector )
{
	ri::Vec2 pointCircleVec = point - circleCenter;
	float distancePointCircleSqrd = pointCircleVec.x * pointCircleVec.x + pointCircleVec.y * pointCircleVec.y;
	float circleRadiusSqrd = circleRadius * circleRadius;

	if(distancePointCircleSqrd <= circleRadiusSqrd) {
		if(collisionVector) *collisionVector = circleCenter - point;
		return true;
	}

	return false;
}



}; // namespace ri