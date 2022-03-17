#pragma once

#include <memory>

#include "Event.h"

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

// Add Controller Event
class AddControllerEvent final : public Event {
private:
	AddControllerEvent(EventEmitter::Ptr controller);

public:
	using Ptr = std::shared_ptr<AddControllerEvent>;
	using UPtr = std::unique_ptr<AddControllerEvent>;
	using WPtr = std::weak_ptr<AddControllerEvent>;
	static const EventType TYPE;

	EventEmitter::Ptr controller;
	static Ptr create(EventEmitter::Ptr controller);
};

// Remove Controller Event
class RemoveControllerEvent final : public Event {
private:
	RemoveControllerEvent(EventEmitter::WPtr controller);

public:
	using Ptr = std::shared_ptr<RemoveControllerEvent>;
	using UPtr = std::unique_ptr<RemoveControllerEvent>;
	using WPtr = std::weak_ptr<RemoveControllerEvent>;
	static const EventType TYPE;

	EventEmitter::WPtr controller;
	static Ptr create(EventEmitter::WPtr controller);
};
