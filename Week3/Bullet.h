#pragma once

#include <functional>

#include "GameObject.h"
#include "NewtonianPhysModel.h"
#include "ImageGraphicsModel.h"

class Bullet : public GameObject {
private:
	static constexpr float SPEED = 40.0f;

	// 2nd phase constructor
	Bullet(
		Vector2D pos, Vector2D rot, PictureIndex image,
		std::shared_ptr<NewtonianPhysModel> physModel
	);

public:
	// Get/Set the right types

	virtual NewtonianPhysModel& physModel() override;
	virtual void setPhysModel(PhysModelPtr physModel) override;

	// Lifecycle

	Bullet(Vector2D pos, Vector2D rot, PictureIndex image);
	virtual ~Bullet();
};
