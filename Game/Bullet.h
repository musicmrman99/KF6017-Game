#pragma once

// Dependencies
  // Basic
#include <functional>
#include "Referencing.h"

  // Objects
#include "Timer.h"

// Traits
#include "GameObject.h"
#include "HasEventEmitterOf.h"
#include "HasEventHandlerOf.h"
#include "HasCollisionOf.h"
#include "HasPhysOf.h"
#include "HasGraphicsOf.h"
#include "ObjectEventCreator.h"

// Models
#include "BufferedEventEmitter.h"
#include "BasicCollisionModel.h"
#include "NewtonianPhysModel.h"
#include "ImageGraphicsModel.h"

// Creation
#include "ObjectFactory.h"
#include "BulletSpec.h"

class Bullet;

class BulletEventHandler final :
	public EventHandler,
	public EventEmitterObserverOf<BufferedEventEmitter>,
	public Referencing<Bullet>,
	public ObjectEventCreator
{
private:
	using BulletWPtr = std::weak_ptr<Bullet>;

	Timer::Ptr timer;

public:
	using UPtr = std::unique_ptr<BulletEventHandler>;

	BulletEventHandler();
	void setTimer(Timer::Ptr timer);
	virtual void handle(const Event::Ptr e) override;
};

class Bullet final :
	public GameObject,
	public HasEventHandlerOf<BulletEventHandler>,
	public HasEventEmitterOf<BufferedEventEmitter>,
	public HasCollisionOf<BasicCollisionModel>,
	public HasPhysOf<NewtonianPhysModel>,
	public HasGraphicsOf<ImageGraphicsModel>,
	public Referencing<Bullet>,
	public ObjectEventCreator
{
private:
	static constexpr float LIFETIME = 1.5f;
	static constexpr float BASE_SPEED = 40.0f;
	static constexpr int COLLISION_RADIUS = 20;

	Timer::Ptr timer;

	Bullet(BulletSpec::UPtr spec);

public:
	using Ptr = std::shared_ptr<Bullet>;
	using UPtr = std::unique_ptr<Bullet>;
	using WPtr = std::weak_ptr<Bullet>;

	// Lifecycle

	static const ObjectFactory factory;

	// FIXME: Dirty hack to make this work for now
	virtual void setObjectEventFactory(ObjectEventFactory::Ptr objectEventFactory) override;

	virtual void afterCreate() override;
};
