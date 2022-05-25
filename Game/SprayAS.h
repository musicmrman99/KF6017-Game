#pragma once

#include "AttackStrategy.h"

class SprayAS : public AttackStrategy {
public:
	static Ptr create();
	virtual void attack(
		std::list<ControlledObject>& controlledObjects,
		std::queue<Event::Ptr>& events
	) override;
};
