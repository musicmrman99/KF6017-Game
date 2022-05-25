#pragma once

// Dependencies
#include <memory>
#include "GameObject.h"
#include "LifecyclePoint.h"

// Traits
#include "HasCollisionOf.h"
#include "HasEventEmitterOf.h"

// Models
#include "BasicCollisionModel.h"
#include "BufferedEventEmitter.h"

// Creation
#include "ObjectFactory.h"
#include "BasicCollisionSpec.h"

// Handles tracking collidable objects and emitting collision events.
// Note that this lifecycle point depends on the Events lifecycle point,
// so is also a GameObject.
class BasicCollision final :
	public LifecyclePoint,
	public GameObject,
	public HasEventEmitterOf<BufferedEventEmitter>
{
private:
	std::list<HasCollisionOf<BasicCollisionModel>::Ptr> collidableObjects;

	BasicCollision(BasicCollisionSpec::UPtr spec);

public:
	using Ptr = std::shared_ptr<BasicCollision>;
	using UPtr = std::unique_ptr<BasicCollision>;
	using WPtr = std::weak_ptr<BasicCollision>;

	static const ObjectFactory factory;

	virtual void objectCreated(GameObject::Ptr object) override;
	virtual void objectDestroyed(GameObject::Ptr object) override;
	virtual void run() override;
};
