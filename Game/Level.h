#pragma once

// Dependencies
#include "Referencing.h"

// Traits
#include "GameObject.h"
#include "HasEventEmitterOf.h"
#include "ObjectEventCreator.h"

#include "LevelActor.h"

// Models
#include "BufferedEventEmitter.h"

// Creation
#include "ObjectFactory.h"
#include "LevelSpec.h"

class Level :
	public LifecyclePoint,
	public GameObject,
	public Referencing<Level>
{
private:
	// Must depend more heavily on the object manager than just creating objects -
	// it must register object event factories at least.
	ObjectManager::Ptr objectManager;

	Level(LevelSpec::Ptr spec);

public:
	using Ptr = std::shared_ptr<Level>;

	static const ObjectFactory factory;

	virtual void afterCreate() override;

	// Implement LifecyclePoint

	virtual void objectCreated(GameObject::Ptr object) override;
	virtual void objectDestroyed(GameObject::Ptr object) override;
	virtual void run() override;
};
