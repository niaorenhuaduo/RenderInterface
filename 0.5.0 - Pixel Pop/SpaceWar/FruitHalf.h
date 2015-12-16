#pragma once

#include "Fruit.h"



class FruitHalf : public ri::Sprite {
public:
	FruitHalf(Fruit::type type, bool right, const ri::Vec2 &position) : Sprite() {
		if(mRockmelonHalf.IsNull()) mRockmelonHalf = ri::ServiceLocator::TexMgr().GetTexture("RockmelonHalf.png");
		if(mWatermelonHalf.IsNull()) mWatermelonHalf = ri::ServiceLocator::TexMgr().GetTexture("WatermelonHalf.png");
		if(mAppleHalf.IsNull()) mAppleHalf = ri::ServiceLocator::TexMgr().GetTexture("AppleHalf.png");

		ri::HTexture texture;

		switch(type) {
		case Fruit::WATERMELON: texture = mWatermelonHalf; break;
		case Fruit::ROCKMELON:  texture = mRockmelonHalf;  break;
		case Fruit::APPLE:		texture = mAppleHalf;      break;
		}

		SetTexture(texture);
		SetAlpha(true);
		SetOrigin(GetLocalBoundingBox().width / 2.0f, GetLocalBoundingBox().height / 2.0f);
		SetPosition(position);

		if(!right) SetFlippingHorizontal(true);

		mStartPos = position;
		mfTimeElapsed = 0.0f;
		mbRight = right;
		mbExpired = false;
	}

	void OnUpdate(double fElapsedTime) {
		Sprite::OnUpdate(fElapsedTime);
		mfTimeElapsed += static_cast<float>(fElapsedTime);


		if(mfTimeElapsed > 3.0f) {
			mbExpired = true;
			return;
		}

		const float temp = ri::PI / 3.5f;

		if(mbRight)  {
			float xPos = 500.0f * sinf(mfTimeElapsed * temp);
			float yPos = 800.0f * pow(mfTimeElapsed - 0.5f, 2) - 130;

			SetPosition(mStartPos + ri::Vec2(xPos, yPos));
			Rotate(static_cast<float>(fElapsedTime) * 250.0f);
		} else {
			float xPos = -800.0f * sinf(mfTimeElapsed * temp);
			float yPos = 800.0f * pow(mfTimeElapsed - 0.5f, 2) - 130;

			SetPosition(mStartPos + ri::Vec2(xPos, yPos));
			Rotate(static_cast<float>(-fElapsedTime) * 250.0f);
		}
	}

	bool IsExpired() const { return mbExpired; }

private:
	bool  mbExpired;
	float mfTimeElapsed;
	bool  mbRight;
	ri::Vec2  mStartPos;

	static ri::HTexture mWatermelonHalf;
	static ri::HTexture mAppleHalf;
	static ri::HTexture mRockmelonHalf;
};