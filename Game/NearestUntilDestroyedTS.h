#pragma once

#include "TargettingStrategy.h"

// For each controlled object individually, the targetter selects the enemy
// nearest to the controlled object, and keeps targetting that enemy until
// it is destroyed. It then reselects the same way.
class NearestUntilDestroyedTS final : public TargettingStrategy {
public:
	static Ptr create();

	virtual void selectTargets(
		std::list<ControlledObject>& controlledObjects,
		const std::list<TargetObject>& targetCandidates
	) override;
};
