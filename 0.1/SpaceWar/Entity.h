#pragma once

#include <RISprite.h>
#include "Input.h"

namespace EntityNS {
	enum CollisionType {
		Circle,
		AABB,
		OBB,
		Pixel,		// ?
	};
}

class Entity : public RISprite {
public:
	Entity();
	virtual ~Entity();

	virtual void VOnInit(RIRenderInterface *pRI, Input *pInput, HTexture texture, U32 width, U32 height,
						 U32 columns, U32 startFrame, U32 endFrame, double timePerFrame, bool bAlpha);
	virtual void VOnInit(RIRenderInterface *pRI, Input *pInput, HTexture texture, const RECT* textureRect = NULL, bool bAlpha = false);


	virtual void VOnUpdate(float elapsedTime);
	virtual bool VTestForCollision(const Entity &other, RIVec2 *pCollisionVec = NULL);
	virtual void VBounce(const Entity &other, const RIVec2 &collisionVec);

	// Set/Get
	void SetCollisionType(EntityNS::CollisionType type);

protected:
	Input *mpInput;

	// Collision
	EntityNS::CollisionType mType;		
	float  mfRadius;		// Radius of the Collision Circle
	float  mfSquaredRadius;
	bool   mbActive;

	// movement
	RIVec2 mVelocity;
	RIVec2 mDeltaVelocity;
	float  mfMass;

private:
	void CalcCollisionData();
};