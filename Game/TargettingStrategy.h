#pragma once

#include "ControlledObject.h"
#include "TargetObject.h"

class TargettingStrategy {
private:
	std::list<TargetObject> _targetCandidates;

public:
	using Ptr = std::shared_ptr<TargettingStrategy>;
	using UPtr = std::unique_ptr<TargettingStrategy>;

	virtual ~TargettingStrategy();

	void addTarget(HasPhysOf<NewtonianPhysModel>::WPtr object);
	const std::list<TargetObject>& targetCandidates();
	virtual void selectTargets(std::list<ControlledObject>& controlledObjects);
};
