#pragma once

// Useful types for AIs

#include <memory>

#include "GameObject.h"
#include "HasPhysOf.h"
#include "NewtonianPhysModel.h"

#include "TargettingData.h"
#include "MovementData.h"
#include "AttackData.h"

struct ControlledObject {
	using Ptr = std::shared_ptr<ControlledObject>;
	using UPtr = std::unique_ptr<ControlledObject>;
	using WPtr = std::weak_ptr<ControlledObject>;

	HasPhysOf<NewtonianPhysModel>::WPtr obj;

	TargettingData::Ptr targettingData;
	MovementData::Ptr movementData;
	AttackData::Ptr attackData;
};
