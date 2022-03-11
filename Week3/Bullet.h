#pragma once

#include <functional>

#include "SelfReferencing.h"

#include "GameObject.h"
#include "NewtonianPhysModel.h"
#include "ImageGraphicsModel.h"

#include "ObjectFactory.h"
#include "BulletSpec.h"
#include "Timer.h"

class Bullet final : public GameObject, public SelfReferencing<Bullet> {
public:
	using Ptr = std::shared_ptr<Bullet>;
	using UPtr = std::unique_ptr<Bullet>;
	using WPtr = std::weak_ptr<Bullet>;

private:
	static constexpr float SPEED = 40.0f;

	Timer::Ptr timer;

	Bullet(BulletSpec::UPtr spec, NewtonianPhysModel::Ptr physModel);

public:
	// Get/Set the right types

	virtual NewtonianPhysModel& physModel() override;
	virtual void setPhysModel(PhysModel::Ptr physModel) override;

	// Lifecycle

	static const ObjectFactory::Factory factory;

	virtual void afterCreate() override;
	virtual void handle(const Event::Ptr e) override;
};
