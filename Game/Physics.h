#pragma once

#include <list>

#include "LifecyclePoint.h"

#include "HasPhys.h"

class Physics : public LifecyclePoint {
private:
	std::list<HasPhys::Ptr> physObjects;

public:
	using Ptr = std::shared_ptr<Physics>;

	static Ptr create();

	virtual void objectCreated(GameObject::Ptr object) override;
	virtual void objectDestroyed(GameObject::Ptr object) override;
	virtual void run() override;
};
