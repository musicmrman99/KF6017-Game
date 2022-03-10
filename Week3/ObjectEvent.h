#pragma once

#include <memory>

#include "GameObject.h"
#include "ObjectManager.h"

// Release Object Event
class ReleaseObjectEvent final : public Event {
private:
	ReleaseObjectEvent(GameObject::UPtr object);

public:
	using Ptr = std::shared_ptr<ReleaseObjectEvent>;
	using UPtr = std::unique_ptr<ReleaseObjectEvent>;
	using WPtr = std::weak_ptr<ReleaseObjectEvent>;

	GameObject::UPtr object;
	static TargettedEvent::UPtr create(ObjectManager::WPtr objectManager, GameObject::UPtr object);
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
	static TargettedEvent::UPtr create(ObjectManager::WPtr objectManager, GameObject* object);
};
