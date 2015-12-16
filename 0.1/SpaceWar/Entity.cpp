#include "Entity.h"


Entity::Entity() : RISprite(), 
	mpInput(NULL), 
	mType(EntityNS::CollisionType::Circle), 
	mfRadius(1.0f),
	mfSquaredRadius(1.0f),
	mbActive(false), 
	mVelocity(0.0f),
	mDeltaVelocity(0.0f),
	mfMass(1.0f) {

}

Entity::~Entity() {}

void Entity::VOnInit( RIRenderInterface *pRI, Input *pInput, HTexture texture, const RECT* textureRect /*= NULL*/, bool bAlpha /*= false*/ ) {
	RISprite::VOnInit(pRI, texture, textureRect, bAlpha);

	assert(pInput && "Entity::VOnInit: pInput == NULL");
	mpInput = pInput;

	CalcCollisionData();
	mbActive = true;
}


void Entity::VOnInit( RIRenderInterface *pRI, Input *pInput, HTexture texture, U32 width, U32 height, U32 columns, U32 startFrame, U32 endFrame, double timePerFrame, bool bAlpha ) {
	RISprite::VOnInit(pRI, texture, width, height, columns, startFrame, endFrame, timePerFrame, bAlpha);

	assert(pInput && "Entity::VOnInit: pInput == NULL");
	mpInput = pInput;

	CalcCollisionData();
	mbActive = true;
}


void Entity::CalcCollisionData() {
	if(!mbInitialized) return;

	mfSquaredRadius = GetRelativeCenter().Dot(GetRelativeCenter());
	mfRadius		= sqrt(mfSquaredRadius);
}



void Entity::VOnUpdate( float elapsedTime ) {
	RISprite::VOnUpdate(elapsedTime);

	mPosition += mVelocity;
	//mVelocity += mAcceleration;

}

bool Entity::VTestForCollision( const Entity &other, RIVec2 *pCollisionVec /*= NULL*/ ) {
	return false;
}

void Entity::VBounce( const Entity &other, const RIVec2 &collisionVec ) {

}