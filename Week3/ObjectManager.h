#pragma once

#include <list>
#include <memory>

#include "GameObject.h"

class ObjectManager final : public EventHandler {
private:
	std::list<GameObject::Ptr> objects;
	std::queue<Event::Ptr> events;

public:
	using Ptr = std::shared_ptr<ObjectManager>;
	// No UPtr - ObjectManager will be onwed by Game, but many EventEmitters (GameObjects, controllers, etc.) may depend on it.
	using WPtr = std::shared_ptr<ObjectManager>;

	// Add a GameObject you uniquely own.
	void addObject(GameObject::UPtr gameObject);

	// Delete any GameObject.
	void deleteObject(GameObject* gameObject);

	// Handle the given object event.
	virtual void handle(const Event::Ptr e) override;

	// Run the lifecycle for all managed objects.
	void run();
};

class ReleaseObjectEvent final : public TargettedEvent {
public:
	static const EventType::Ptr EVENT_TYPE;

	GameObject::UPtr object;
	ReleaseObjectEvent(ObjectManager::Ptr objectManager, GameObject::UPtr object);
};

class DestroyObjectEvent final : public TargettedEvent {
public:
	static const EventType::Ptr EVENT_TYPE;

	GameObject* object;
	DestroyObjectEvent(ObjectManager::Ptr objectManager, GameObject* object);
};
