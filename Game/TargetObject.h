#pragma once

#include "HasPhysOf.h"
#include "NewtonianPhysModel.h"

struct TargetObject {
	using Ptr = std::shared_ptr<TargetObject>;
	using UPtr = std::unique_ptr<TargetObject>;
	using WPtr = std::weak_ptr<TargetObject>;

	HasPhysOf<NewtonianPhysModel>::WPtr obj;
};
