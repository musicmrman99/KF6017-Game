#pragma once

#include "TargettingData.h"
#include "TargetObject.h"

class NearestUntilDestroyedTD : public TargettingData {
public:
	using Ptr = std::shared_ptr<NearestUntilDestroyedTD>;

	static Ptr create();

	TargetObject objTarget;
};
