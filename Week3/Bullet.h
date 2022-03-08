#pragma once

#include <functional>

#include "GameObject.h"
#include "NewtonianPhysModel.h"
#include "ImageGraphicsModel.h"

class Bullet final : public GameObject {
private:
	static constexpr float SPEED = 40.0f;

	// 2nd phase constructor
	Bullet(
		Vector2D pos, Vector2D rot, PictureIndex image,
		NewtonianPhysModel::Ptr physModel
	);

public:
	// Get/Set the right types

	virtual NewtonianPhysModel& physModel() override;
	virtual void setPhysModel(PhysModel::Ptr physModel) override;

	// Lifecycle

	Bullet(Vector2D pos, Vector2D rot, PictureIndex image);
	virtual ~Bullet();
};
