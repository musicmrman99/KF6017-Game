#pragma once

#include <memory>

#include "Event.h"
#include "EventEmitter.h"

struct ObjectSpec;
class GameObject;

// Release Object Event
class CreateObjectEvent final : public Event {
private:
	using ObjectSpecUPtr = std::unique_ptr<ObjectSpec>;

	CreateObjectEvent(ObjectSpecUPtr spec);

public:
	using Ptr = std::shared_ptr<CreateObjectEvent>;
	using UPtr = std::unique_ptr<CreateObjectEvent>;
	using WPtr = std::weak_ptr<CreateObjectEvent>;
	static const EventType TYPE;

	ObjectSpecUPtr spec;
	static Ptr create(ObjectSpecUPtr spec);
};

// Destroy Object Event
class DestroyObjectEvent final : public Event {
private:
	using GameObjectWPtr = std::weak_ptr<GameObject>;

	DestroyObjectEvent(GameObjectWPtr object);

public:
	using Ptr = std::shared_ptr<DestroyObjectEvent>;
	using UPtr = std::unique_ptr<DestroyObjectEvent>;
	using WPtr = std::weak_ptr<DestroyObjectEvent>;
	static const EventType TYPE;

	GameObjectWPtr object;
	static Ptr create(GameObjectWPtr object);
};
