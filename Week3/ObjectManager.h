#pragma once

#include <list>
#include <memory>

#include "GameObject.h"

// Object Manager
class ObjectManager final : public EventHandler {
private:
	std::list<GameObject::Ptr> objects;
	std::queue<Event::Ptr> events;

public:
	using Ptr = std::shared_ptr<ObjectManager>;
	// No UPtr - ObjectManager will be onwed by Game, but many EventEmitters (GameObjects, controllers, etc.) may depend on it.
	using WPtr = std::weak_ptr<ObjectManager>;

	// Add a GameObject you uniquely own.
	void addObject(GameObject::UPtr gameObject);

	// Delete any GameObject.
	void deleteObject(GameObject* gameObject);

	// Handle the given object event.
	virtual void handle(const Event::Ptr e) override;

	// Run the lifecycle for all managed objects.
	void run();
};

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
