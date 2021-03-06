#pragma once

#include <memory>

#include "TargettedEvent.h"
#include "EventEmitter.h"

struct ObjectSpec;
class GameObject;
class ObjectManager;

class ObjectEventFactory {
private:
	using ObjectManagerWPtr = std::weak_ptr<ObjectManager>;
	using GameObjectWPtr = std::weak_ptr<GameObject>;
	using ObjectSpecUPtr = std::unique_ptr<ObjectSpec>;

	ObjectManagerWPtr objectManager;

	ObjectEventFactory(ObjectManagerWPtr objectManager);

public:
	using Ptr = std::shared_ptr<ObjectEventFactory>;

	static Ptr create(ObjectManagerWPtr objectManager);

	TargettedEvent::UPtr createObject(ObjectSpecUPtr spec) const;
	TargettedEvent::UPtr destroyObject(GameObjectWPtr object) const;
};
