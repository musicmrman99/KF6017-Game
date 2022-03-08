#pragma once

#include <functional>

#include "ObjectManager.h"
#include "GameObject.h"
#include "NewtonianPhysModel.h"
#include "ImageGraphicsModel.h"

class Bullet final : public GameObject {
private:
	static constexpr float SPEED = 40.0f;
	
	ObjectManager::WPtr objectManager;
	std::queue<Event::Ptr> globalEventBuffer;

	// 2nd phase constructor
	Bullet(
		Vector2D pos, Vector2D rot, PictureIndex image, ObjectManager::WPtr objectManager,
		NewtonianPhysModel::Ptr physModel
	);

public:
	// Get/Set the right types

	virtual NewtonianPhysModel& physModel() override;
	virtual void setPhysModel(PhysModel::Ptr physModel) override;

	// Lifecycle

	Bullet(Vector2D pos, Vector2D rot, PictureIndex image, ObjectManager::WPtr objectManager);
	virtual void handle(const Event::Ptr e) override;
	virtual void emit(std::queue<Event::Ptr>& globalEvents) override;
};
