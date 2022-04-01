#pragma once

#include <functional>

#include "Referencing.h"

#include "GameObject.h"
#include "HasEventEmitterOf.h"
#include "HasEventHandlerOf.h"
#include "HasCollisionOf.h"
#include "HasPhysOf.h"
#include "HasGraphicsOf.h"

#include "ObjectEventCreator.h"
#include "BufferedEventEmitter.h"
#include "BasicCollisionModel.h"
#include "NewtonianPhysModel.h"
#include "ImageGraphicsModel.h"

#include "ObjectFactory.h"
#include "BulletSpec.h"
#include "Timer.h"

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
	static constexpr float SPEED = 40.0f;
	static constexpr float OBJECT_CULL_TIME = 1.5f;

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
	virtual void beforeDraw() override;
};
