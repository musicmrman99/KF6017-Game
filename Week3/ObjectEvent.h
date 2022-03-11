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
