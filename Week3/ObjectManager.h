#pragma once

#include <list>
#include <memory>

#include "GameObject.h"
#include "ObjectSpec.h"

// Object Manager
class ObjectManager final : public EventHandler {
private:
	std::list<EventEmitter::Ptr> controllers;
	std::list<GameObject::Ptr> objects;
	std::queue<Event::Ptr> events;

public:
	using Ptr = std::shared_ptr<ObjectManager>;
	// No UPtr - ObjectManager will be onwed by Game, but many EventEmitters (GameObjects, controllers, etc.) may depend on it.
	using WPtr = std::weak_ptr<ObjectManager>;

	// Create a GameObject.
	GameObject::Ptr createObject(ObjectSpec::UPtr spec);

	// Destroy any GameObject.
	void destroyObject(GameObject* object);

	// Add a GameObject you uniquely own.
	void addController(EventEmitter::Ptr controller);

	// Delete any GameObject.
	void removeController(EventEmitter* controller);

	// Handle the given object event.
	virtual void handle(const Event::Ptr e) override;

	// Run the lifecycle for all managed objects.
	void run();
};
