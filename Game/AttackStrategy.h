#pragma once

#include "ControlledObject.h"
#include "Event.h"

class AttackStrategy {
public:
	using Ptr = std::shared_ptr<AttackStrategy>;
	using UPtr = std::unique_ptr<AttackStrategy>;

	virtual ~AttackStrategy();

	virtual void attack(
		std::list<ControlledObject>& controlledObjects,
		std::queue<Event::Ptr>& events
	) = 0;
};
