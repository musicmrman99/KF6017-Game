#pragma once

#include <list>
#include <memory>

#include "Referencing.h"

#include "GameObject.h"
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
	// No UPtr - ObjectManager will be onwed by Game, but many EventEmitters (GameObjects) may depend on it.
	using WPtr = std::weak_ptr<ObjectManager>;

private:
	ObjectEventFactory::Ptr objectEventFactory;
	ObjectFactoryManager factory;

	std::list<GameObject::Ptr> objects;
	std::list<HasEventEmitter::Ptr> eventEmitters;
	std::list<HasEventHandler::Ptr> eventHandlers;
	std::list<HasPhys::Ptr> physObjects;
	std::list<HasGraphics::Ptr> graphicsObjects;
	std::list<HasUI::Ptr> uiObjects;
	
	std::queue<Event::Ptr> events;
	
	ObjectManager();
	virtual void setRef(WPtr self) override;

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

	// Handle the given object event.
	virtual void handle(const Event::Ptr e) override;

	/* Frame Process
	-------------------- */

	// Run the lifecycle for all managed objects.
	void run();
};
