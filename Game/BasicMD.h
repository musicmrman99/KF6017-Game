#pragma once

#include "vector2D.h"

#include "MovementData.h"

class BasicMD : public MovementData {
private:
	BasicMD(
		float maximumSpeed,
		float accelMaxAngle,
		float targetDistance,
		float targetRotVel,
		float targetOffsetAmplitude,
		float targetOffsetFrequency
	);

public:
	using Ptr = std::shared_ptr<BasicMD>;

	static Ptr create(
		float maximumSpeed,
		float accelMaxAngle,
		float targetDistance,
		float targetRotVel,
		float targetOffsetAmplitude,
		float targetOffsetFrequency
	);

	float maximumSpeed;
	float accelMaxAngle;

	/*
				 |_
				 |  \ rotation
				 a   |
				/    |
			   /\ _ /
	 distance /
			 /      _b
		   |/_      /|
		   O  +/-  / offset (based on sin() of rotation in parallel direction to distance)
				 |/_
				 b
	*/
	float targetRot;
	float targetDistance;

	float targetRotVel;
	float targetOffsetAmplitude;
	float targetOffsetFrequency;
};
