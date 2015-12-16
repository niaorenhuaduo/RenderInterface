#pragma once

#include "Sprite.h"


namespace ri {

////////////////////////////////////////////////////////////
/// \brief A sprite with the ability to collide with other sprites
///
/// Req.: *The origin of the sprite class has to be the center of the sprite, otherwise the collision detection won't be accurate
///		  *The sprite radius and Edge rect have to be set manually
///
////////////////////////////////////////////////////////////
class RI_EXPORT Entity : public Sprite {
public: 

	enum CollisionType {
		CT_NONE,
		CT_CIRCLE,
		CT_AABB,
		CT_OOBB
		//CT_PIXEL
	};

	////////////////////////////////////////////////////////////
	/// \brief Constructors
	///
	////////////////////////////////////////////////////////////
	Entity();
	Entity(ri::HTexture tex, bool bAlpha = false);
	Entity(ri::HTexture tex, const ri::IntRect &textureRect, bool bAlpha = false);
	Entity(ri::HTexture tex, U32 frameWidth, U32 frameHeight, U32 numColumns, U32 startFrame, U32 endFrame, double frameDelay, bool bLooped = false, bool bAlpha = false);



	////////////////////////////////////////////////////////////
	/// \brief Updates the physics and animations
	///
	////////////////////////////////////////////////////////////
	virtual void OnUpdate(double fElapsedTime);


	////////////////////////////////////////////////////////////
	/// \brief Test for an collision with the other entity
	///
	////////////////////////////////////////////////////////////
	bool CollidesWith(const Entity &ent, ri::Vec2 *collisionVector);

	////////////////////////////////////////////////////////////
	/// \brief Bounces this entity of the other entity based on mass/collisionVector;
	///
	////////////////////////////////////////////////////////////
	void Bounce(const ri::Vec2 &collisionVector, const Entity &ent);



	void SetCollisionType(CollisionType type) { mType = type; }
	CollisionType GetCollisionType() const { return mType; }

	////////////////////////////////////////////////////////////
	/// \brief Sets the velocity/direction of the entity
	///
	////////////////////////////////////////////////////////////
	void SetVelocity(const ri::Vec2 &vel) { mVelocity = vel; }

	////////////////////////////////////////////////////////////
	/// \brief Returns the velocity/direction of the entity
	///
	////////////////////////////////////////////////////////////
	const ri::Vec2& GetVelocity() const { return mVelocity; }


	////////////////////////////////////////////////////////////
	/// \brief Activates/Deactivates the entity for collision tests
	///
	////////////////////////////////////////////////////////////
	void SetActive(bool bActive) { mbActive = bActive; }

	////////////////////////////////////////////////////////////
	/// \brief Returns if the entity is activated for collisions or not
	///
	////////////////////////////////////////////////////////////
	bool GetActive() const { return mbActive;}

	////////////////////////////////////////////////////////////
	/// \brief Sets the unscaled radius of the sprite, default is 1.0f
	///
	////////////////////////////////////////////////////////////
	void SetRadius(float radius) { mfRadius = abs(radius); }

	////////////////////////////////////////////////////////////
	/// \brief Returns the unscaled radius of the sprite
	///
	////////////////////////////////////////////////////////////
	float GetRadius() const { return mfRadius; }

	////////////////////////////////////////////////////////////
	/// \brief Sets the mass of this entity; 0 for stationary objects
	///
	////////////////////////////////////////////////////////////
	void SetMass(float fMass) { mfMass = fMass; }

	////////////////////////////////////////////////////////////
	/// \brief Returns the mass of this entity; 0 for stationary objects
	///
	////////////////////////////////////////////////////////////
	float GetMass() const { return mfMass; }

	////////////////////////////////////////////////////////////
	/// \brief Set the unscaled Edges of the entity, default values = {-1, 1}
	///
	/// \param The Edges should be relative to the entity center
	///
	////////////////////////////////////////////////////////////
	void SetEdge(const RECT &rect) { mEdge = rect; }

	////////////////////////////////////////////////////////////
	/// \brief Returns a rect containing the unscaled relative positions of the edges to the center
	///
	/// \param Default values are {-1, 1} 
	///
	////////////////////////////////////////////////////////////
	const RECT& GetEdge() const { return mEdge; }




	////////////////////////////////////////////////////////////
	/// \brief Returns the vector of one corner for rotated box computations
	///
	////////////////////////////////////////////////////////////
	const ri::Vec2& GetCorner(UINT c) const
	{
		if(c>=4) 
			c=0;
		return mCorners[c]; 
	}

protected:

	CollisionType mType;

	ri::Vec2	mVelocity;
	ri::Vec2	mDeltaVelocity;
	float		mfMass;
	bool		mbActive;

	// circle collision
	float		mfRadius;			// You have to initialize the radius yourself

	// rotated box collision
	RECT		mEdge;
	mutable ri::Vec2	mCorners[4];
	mutable ri::Vec2	mEdge01, mEdge03;
	mutable float		mEdge01Min, mEdge01Max, mEdge03Min, mEdge03Max;
	mutable bool		mbRotatedBoxReady;

	void Initialize();
	void ComputeRotatedBox() const;
	bool ProjectionsOverlap(const Entity &ent) const;

	friend bool EntityCollideOOBB(const Entity &ent1, const Entity &ent2, ri::Vec2 *collisionVector);
	friend bool EntityCollideBoxCircle(const Entity &ent1, const Entity &ent2, ri::Vec2 *collisionVector);
};

////////////////////////////////////////////////////////////
/// \brief returns true if the two entities collide. collisionVector represents a vector between the two entities
///
////////////////////////////////////////////////////////////
bool EntityCollideCircle(const Entity &ent1, const Entity &ent2, ri::Vec2 *collisionVector);

////////////////////////////////////////////////////////////
/// \brief returns true if the two entities collide. collisionVector represents a vector between the two entities
///
////////////////////////////////////////////////////////////
bool EntityCollideAABB(const Entity &ent1, const Entity &ent2, ri::Vec2 *collisionVector);

////////////////////////////////////////////////////////////
/// \brief returns true if the two entities collide. collisionVector represents a vector between the two entities
///
////////////////////////////////////////////////////////////
bool EntityCollideOOBB(const Entity &ent1, const Entity &ent2, ri::Vec2 *collisionVector);

////////////////////////////////////////////////////////////
/// \brief returns true if the two entities collide. collisionVector represents a vector between the two entities
///
/// \param ent1 This entity must be using AABB/OOBB collision
/// \param ent2 This entity must be using circle collision
///
////////////////////////////////////////////////////////////
bool EntityCollideBoxCircle(const Entity &ent1, const Entity &ent2, ri::Vec2 *collisionVector);

////////////////////////////////////////////////////////////
/// \brief Returns true if the point is in the area of the circle. collisionVector points from point to circleCenter
///
////////////////////////////////////////////////////////////
bool PointCollidesCircle(const ri::Vec2 &point, const ri::Vec2 &circleCenter, float circleRadius, ri::Vec2 *collisionVector);


}; // namespace ri