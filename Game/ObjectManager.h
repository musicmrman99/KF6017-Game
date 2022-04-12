#pragma once

#include <list>
#include <memory>

#include "Referencing.h"

#include "GameObject.h"
#include "LifecyclePoint.h"

#include "HasEventEmitter.h"
#include "HasEventHandler.h"
#include "HasPhys.h"
#include "HasGraphics.h"
#include "HasUI.h"

#include "ObjectSpec.h"
#include "ObjectFactoryManager.h"
#include "ObjectEventFactory.h"

// Object Manager
class ObjectManager final : public EventHandler, public Referencing<ObjectManager> {
public:
	using Ptr = std::shared_ptr<ObjectManager>;
	// No UPtr - ObjectManager will be onwed by Game, but many other objects may depend on it.
	using WPtr = std::weak_ptr<ObjectManager>;

private:
	std::list<LifecyclePoint::Ptr> lifecyclePoints;
	std::list<GameObject::Ptr> objects;

	ObjectEventFactory::Ptr objectEventFactory;
	ObjectFactoryManager factory;

	ObjectManager();
	virtual void setRef(WPtr self) override;

public:
	/* Creation
	-------------------- */

	static Ptr create();

	/* Getters
	-------------------- */

	ObjectFactoryManager& getObjectFactoryManager();
	std::list<GameObject::Ptr>& getAllGameObjects();

	/* Lifecycle Points
	-------------------- */

	// Add a lifecycle point. These allow instances of a special kind
	// of GameObject to track object creation/destruction events.
	// Once added, you cannot remove a lifecycle point - they are
	// global to a game. The only reason you have to add the lifecycle
	// points provided by the engine is so that you can dictate their
	// order of execution in the main game loop.
	void addLifecyclePoint(LifecyclePoint::Ptr lifecyclePoint);

	/* Event Handling
	-------------------- */

	// Create a GameObject.
	GameObject::Ptr createObject(ObjectSpec::UPtr spec);

	// Destroy any GameObject.
	void destroyObject(GameObject::WPtr object);

	// Handle the given object event.
	virtual void handle(const Event::Ptr e) override;

	/* Frame Process
	-------------------- */

	// Run the lifecycle for all managed objects.
	void run();
};
