#pragma once

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

class Level final :
	public LifecyclePoint,
	public GameObject,
	public HasEventEmitterOf<BufferedEventEmitter>
{
private:
	// As a LifecyclePoint, tracks all GameObjects that need it
	std::list<LevelActor::Ptr> levelActors;

	// Must depend more heavily on the object manager than just creating objects -
	// it must register object event factories at least.
	ObjectManager::Ptr objectManager;

	Level(LevelSpec::Ptr spec);

public:
	using Ptr = std::shared_ptr<Level>;

	static const ObjectFactory factory;

	// Allow (actually, require) overriding all GameObject methods

	virtual void afterCreate() override;
	virtual void beforeFrame() override;
	virtual void afterFrame() override;
	virtual void beforeDestroy() override;

	// Implement LifecyclePoint

	virtual void objectCreated(GameObject::Ptr object) override;
	virtual void objectDestroyed(GameObject::Ptr object) override;
	virtual void run() override;
};
