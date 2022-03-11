#pragma once

#include <functional>

#include "GameObject.h"
#include "NewtonianPhysModel.h"
#include "ImageGraphicsModel.h"

#include "ObjectFactory.h"
#include "BulletSpec.h"

class Bullet final : public GameObject {
private:
	static constexpr float SPEED = 40.0f;
	
	std::queue<Event::Ptr> globalEventBuffer;

	// 2nd phase constructor
	Bullet(BulletSpec::UPtr spec, NewtonianPhysModel::Ptr physModel);

public:
	// Get/Set the right types

	virtual NewtonianPhysModel& physModel() override;
	virtual void setPhysModel(PhysModel::Ptr physModel) override;

	// Lifecycle

	Bullet(BulletSpec::UPtr bullet);
	static const ObjectFactory::Factory factory;

	virtual void handle(const Event::Ptr e) override;
	virtual void emit(std::queue<Event::Ptr>& globalEvents) override;
};
