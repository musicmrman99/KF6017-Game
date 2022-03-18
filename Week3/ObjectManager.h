#pragma once

#include <list>
#include <memory>

#include "SelfReferencing.h"

#include "GameObject.h"
#include "HasPhys.h"

#include "ObjectSpec.h"
#include "ObjectFactoryManager.h"
#include "ObjectEventFactory.h"

// Object Manager
class ObjectManager final : public EventHandler, public SelfReferencing<ObjectManager> {
public:
	using Ptr = std::shared_ptr<ObjectManager>;
	// No UPtr - ObjectManager will be onwed by Game, but many EventEmitters (GameObjects, controllers, etc.) may depend on it.
	using WPtr = std::weak_ptr<ObjectManager>;

private:
	ObjectEventFactory::Ptr objectEventFactory;
	ObjectFactoryManager factory;

	std::list<EventEmitter::Ptr> controllers;

	std::list<GameObject::Ptr> objects;
	std::list<HasPhys::Ptr> physObjects;
	
	std::queue<Event::Ptr> events;
	
	ObjectManager();
	virtual void setSelf(WPtr self) override;

public:
	/* Creation
	-------------------- */

	static Ptr create();

	/* Getters
	-------------------- */

	ObjectFactoryManager& getObjectFactoryManager();

	/* Event Handling
	-------------------- */

	// Create a GameObject.
	GameObject::Ptr createObject(ObjectSpec::UPtr spec);

	// Destroy any GameObject.
	void destroyObject(GameObject::WPtr object);

	// Add a GameObject you uniquely own.
	void addController(EventEmitter::Ptr controller);

	// Delete any GameObject.
	void removeController(EventEmitter::WPtr controller);

	// Handle the given object event.
	virtual void handle(const Event::Ptr e) override;

	/* Frame Process
	-------------------- */

	// Run the lifecycle for all managed objects.
	void run();
};
