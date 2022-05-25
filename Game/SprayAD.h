#pragma once

#include "AttackData.h"

class SprayAD : public AttackData {
private:
	SprayAD(
		float maxAttackAngle,
		bool canRotate,
		float firingAngleRotationSpeed
	);

public:
	using Ptr = std::shared_ptr<SprayAD>;

	static Ptr create(
		float maxAttackAngle,
		bool canRotate,
		float firingAngleRotationSpeed
	);

	float maxAttackAngle;
	bool canRotate;
	float firingAngleRotationSpeed;
};
