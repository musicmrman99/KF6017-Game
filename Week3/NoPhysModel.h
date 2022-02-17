#pragma once

#include "PhysModel.h"

class NoPhysModel : public PhysModel {
public:
	virtual void run() override;
};
