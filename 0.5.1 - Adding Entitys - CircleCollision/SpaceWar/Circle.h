#pragma once

#include <ServiceLocator.h>

#include <Drawable\Entity.h>
#include "Constants.h"

class Circle : public ri::Entity {
public:

	Circle() : Entity() {
		if(tex.IsNull()) tex = ri::ServiceLocator::TexMgr().GetTexture("bubble.png");

		SetTexture(tex);
		SetAlpha(true);

		SetOrigin(CIRCLE_RECT.width / 2.0f, CIRCLE_RECT.height / 2.0f);
		SetRadius(CIRCLE_RADIUS);
		SetCollisionType(Entity::CT_CIRCLE);
		SetMass(1.0f);
	}

	void OnUpdate(double fElapsedTime) {
		Entity::OnUpdate(fElapsedTime);

		float radius = GetRadius();
		ri::Vec2 pos = GetPosition();
		if(pos.x - radius < 0.0f) {
			SetPosition(radius, pos.y);
			mVelocity.x *= -1.0f;
		} else if(pos.x + radius > ri::VIRTUALXRESOLUTION) {
			SetPosition(ri::VIRTUALXRESOLUTION - radius, pos.y);
			mVelocity.x *= -1.0f;
		}

		if(pos.y - radius < 0.0f) {
			SetPosition(pos.x, radius);
			mVelocity.y *= -1.0f;
		} else if(pos.y + radius > ri::VIRTUALYRESOLUTION) {
			SetPosition(pos.x, ri::VIRTUALYRESOLUTION - radius);
			mVelocity.y *= -1.0f;
		}
	}

private:

	static ri::HTexture tex;
};