#pragma once

#include <memory>

#include "GameObject.h"
#include "ObjectSpec.h"

// Release Object Event
class CreateObjectEvent final : public Event {
private:
	CreateObjectEvent(ObjectSpec::UPtr spec);

public:
	using Ptr = std::shared_ptr<CreateObjectEvent>;
	using UPtr = std::unique_ptr<CreateObjectEvent>;
	using WPtr = std::weak_ptr<CreateObjectEvent>;

	ObjectSpec::UPtr spec;
	static UPtr create(ObjectSpec::UPtr spec);
};

// Destroy Object Event
class DestroyObjectEvent final : public Event {
private:
	DestroyObjectEvent(GameObject* object);

public:
	using Ptr = std::shared_ptr<DestroyObjectEvent>;
	using UPtr = std::unique_ptr<DestroyObjectEvent>;
	using WPtr = std::weak_ptr<DestroyObjectEvent>;

	GameObject* object;
	static UPtr create(GameObject* object);
};

// Add Controller Event
class AddControllerEvent final : public Event {
private:
	AddControllerEvent(EventEmitter::Ptr controller);

public:
	using Ptr = std::shared_ptr<AddControllerEvent>;
	using UPtr = std::unique_ptr<AddControllerEvent>;
	using WPtr = std::weak_ptr<AddControllerEvent>;

	EventEmitter::Ptr controller;
	static UPtr create(EventEmitter::Ptr controller);
};

// Remove Controller Event
class RemoveControllerEvent final : public Event {
private:
	RemoveControllerEvent(EventEmitter* controller);

public:
	using Ptr = std::shared_ptr<RemoveControllerEvent>;
	using UPtr = std::unique_ptr<RemoveControllerEvent>;
	using WPtr = std::weak_ptr<RemoveControllerEvent>;

	EventEmitter* controller;
	static UPtr create(EventEmitter* controller);
};
