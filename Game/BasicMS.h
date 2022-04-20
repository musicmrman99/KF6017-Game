#pragma once

#include "MovementStrategy.h"
#include "BasicMovement.h"

// Roughly tries to:
// 1. Face the target so it can fire at it.
// 2. Avoid colliding with the target itself.
class BasicMS final : public MovementStrategy {
public:
	static Ptr create();

	virtual void moveObjects(
		std::list<ControlledObject>& controlledObjects,
		std::queue<Event::Ptr>& events
	) override;
};
