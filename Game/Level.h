#pragma once

// Dependencies
#include <queue>
#include <map>
#include <string>
#include "Referencing.h"

#include "PlayerShip.h"
#include "StarField.h"

#include "BasicAI.h"

// Traits
#include "GameObject.h"
#include "HasEventEmitterOf.h"
#include "ObjectEventCreator.h"

#include "LevelActor.h"
#include "HasPhysOf.h"
#include "NewtonianPhysModel.h"

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
	ObjectManager::WPtr objectManager;
	Level(LevelSpec::Ptr spec);

	// Camera
	Vector2D cameraOffset;
	HasPhysOf<NewtonianPhysModel>::Ptr cameraFocusObject;
	static const float CAMERA_SHIFT;
	static const float CAMERA_ELASTICITY;

	// Services owned by the level manager
	std::map<std::string, PictureIndex> spriteMap;

	// Global entities
	PlayerShip::Ptr player;
	StarField::Ptr starField;

public:
	using Ptr = std::shared_ptr<Level>;

	static const ObjectFactory factory;

	virtual void afterCreate() override;
	
	// Level Management

	BasicAI::Ptr fighterAI;
	void spawnFighter();

	// Global State

	void setCameraFocus(HasPhysOf<NewtonianPhysModel>::Ptr physObject);

	// Implement LifecyclePoint

	virtual void objectCreated(GameObject::Ptr object) override;
	virtual void run() override;
};
