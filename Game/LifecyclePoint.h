#pragma once

#include <memory>
#include <list>

#include "GameObject.h"

class LifecyclePoint {
public:
	using Ptr = std::shared_ptr<LifecyclePoint>;

	virtual ~LifecyclePoint();

	virtual void objectCreated(GameObject::Ptr object) = 0;
	virtual void objectDestroyed(GameObject::Ptr object) = 0;

	virtual void run() = 0;
};
