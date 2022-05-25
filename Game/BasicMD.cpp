#include "BasicMD.h"

BasicMD::BasicMD(
	float maximumSpeed,
	float targetDistance,
	float targetRotVel,
	float targetOffsetAmplitude,
	float targetOffsetFrequency
) :
	maximumSpeed(maximumSpeed),
	targetRot(0.0f),
	targetDistance(targetDistance),
	targetRotVel(targetRotVel),
	targetOffsetAmplitude(targetOffsetAmplitude),
	targetOffsetFrequency(targetOffsetFrequency)
{}

BasicMD::Ptr BasicMD::create(
	float maximumSpeed,
	float targetDistance,
	float targetRotVel,
	float targetOffsetAmplitude,
	float targetOffsetFrequency
) {
	return Ptr(new BasicMD(
		maximumSpeed,
		targetDistance,
		targetRotVel,
		targetOffsetAmplitude,
		targetOffsetFrequency
	));
}
