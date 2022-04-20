#pragma once

#include "AITypes.h"

class TargettingStrategy {
public:
	using Ptr = std::shared_ptr<TargettingStrategy>;
	using UPtr = std::unique_ptr<TargettingStrategy>;

	virtual ~TargettingStrategy();

	virtual void selectTargets(
		std::list<ControlledObject>& controlledObjects,
		const std::list<TargetObject>& targetCandidates
	) = 0;
};
