#pragma once

// Dependencies
#include <queue>
#include "Referencing.h"

#include "Ship.h"
#include "StarField.h"

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
	HasPhysOf<NewtonianPhysModel>::Ptr cameraFocusObject;
	std::queue<Vector2D> latencyQueue;
	static const int LATENCY = 30;

	// Global entities
	Ship::Ptr player;
	StarField::Ptr starField;

public:
	using Ptr = std::shared_ptr<Level>;

	static const ObjectFactory factory;

	virtual void afterCreate() override;

	// Modifying global state

	void setCameraFocus(HasPhysOf<NewtonianPhysModel>::Ptr physObject);

	// Implement LifecyclePoint

	virtual void objectCreated(GameObject::Ptr object) override;
	virtual void run() override;
};