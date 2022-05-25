#include "BasicMD.h"

BasicMD::BasicMD(
	float targetDistance,
	float targetRotVel,
	float targetOffsetAmplitude,
	float targetOffsetFrequency
) :
	targetRot(0.0f),
	targetDistance(targetDistance),

	targetRotVel(targetRotVel),
	targetOffsetAmplitude(targetOffsetAmplitude),
	targetOffsetFrequency(targetOffsetFrequency)
{}

BasicMD::Ptr BasicMD::create(
	float targetDistance,
	float targetRotVel,
	float targetOffsetAmplitude,
	float targetOffsetFrequency
) {
	return Ptr(new BasicMD(
		targetDistance,
		targetRotVel,
		targetOffsetAmplitude,
		targetOffsetFrequency
	));
}
