#pragma once

#include <list>

#include "LifecyclePoint.h"

#include "HasEventEmitter.h"
#include "HasEventHandler.h"

class Events : public LifecyclePoint {
private:
	std::list<HasEventEmitter::Ptr> eventEmitters;
	std::list<HasEventHandler::Ptr> eventHandlers;

	std::queue<Event::Ptr> events;

public:
	using Ptr = std::shared_ptr<Events>;

	static Ptr create();

	virtual void objectCreated(GameObject::Ptr object) override;
	virtual void objectDestroyed(GameObject::Ptr object) override;
	virtual void run() override;
};
