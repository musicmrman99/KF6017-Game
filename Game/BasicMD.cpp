#include "BasicMD.h"

BasicMD::BasicMD(
	float maximumSpeed,
	float accelMaxAngle,
	float targetDistance,
	float targetRotVel,
	float targetOffsetAmplitude,
	float targetOffsetFrequency
) :
	maximumSpeed(maximumSpeed),
	accelMaxAngle(accelMaxAngle),
	targetRot(0.0f),
	targetDistance(targetDistance),
	targetRotVel(targetRotVel),
	targetOffsetAmplitude(targetOffsetAmplitude),
	targetOffsetFrequency(targetOffsetFrequency)
{}

BasicMD::Ptr BasicMD::create(
	float maximumSpeed,
	float accelMaxAngle,
	float targetDistance,
	float targetRotVel,
	float targetOffsetAmplitude,
	float targetOffsetFrequency
) {
	return Ptr(new BasicMD(
		maximumSpeed,
		accelMaxAngle,
		targetDistance,
		targetRotVel,
		targetOffsetAmplitude,
		targetOffsetFrequency
	));
}
