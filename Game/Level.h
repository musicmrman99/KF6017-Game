#pragma once

// Traits
#include "GameObject.h"
#include "HasEventEmitterOf.h"
#include "ObjectEventCreator.h"

// Models
#include "BufferedEventEmitter.h"

// Creation
#include "ObjectFactory.h"
#include "LevelSpec.h"

class Level final :
	public GameObject,
	public HasEventEmitterOf<BufferedEventEmitter>
{
private:

	// Must depend more heavily on the object manager than just creating objects -
	// it must register object event factories, and may need to do complex threading
	// of dependencies for global objects, etc.
	ObjectManager::Ptr objectManager;

	Level(LevelSpec::Ptr spec);

public:
	static const ObjectFactory factory;

	virtual void afterCreate() override;
};
