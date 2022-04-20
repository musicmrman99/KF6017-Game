#pragma once

#include "AITypes.h"
#include "Event.h"

class MovementStrategy {
public:
	using Ptr = std::shared_ptr<MovementStrategy>;
	using UPtr = std::unique_ptr<MovementStrategy>;

	virtual ~MovementStrategy();

	virtual void moveObjects(
		std::list<ControlledObject>& controlledObjects,
		std::queue<Event::Ptr>& events
	) = 0;
};
