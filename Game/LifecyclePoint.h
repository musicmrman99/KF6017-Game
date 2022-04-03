#pragma once

#include <memory>
#include <list>

#include "GameObject.h"

// A phase of update in frame.
// This interface allows you to track GameObjects added to and removed from the object manager.
// Note: objectCreated() will not be called on LifecyclePoints for objects that already
//   exist in the object manager at the time the lifecycle point is added. It is therefore
//   necessary to carefully order the creation of lifecycle points if they depend directly on
//   being able to talk to other GameObjects that are also lifecycle points.
class LifecyclePoint {
public:
	using Ptr = std::shared_ptr<LifecyclePoint>;

	virtual ~LifecyclePoint();

	virtual void objectCreated(GameObject::Ptr object);
	virtual void objectDestroyed(GameObject::Ptr object);
	virtual void run();
};
