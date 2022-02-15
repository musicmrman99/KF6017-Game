#pragma once

#include "Vector2D.h"

#include "PhysModel.h"

class NewtonianPhysModel : public PhysModel {
private:
	Vector2D _pos;
	Vector2D _vel;
	Vector2D _accel;

	Vector2D _rot;
	float _rotVel;
	float _rotAccel;

public:
	// Constructors

	NewtonianPhysModel(Vector2D pos, Vector2D vel, Vector2D accel, Vector2D rot, float rotVel, float rotAccel);
	NewtonianPhysModel(Vector2D pos, Vector2D vel, Vector2D rot, float rotVel);
	NewtonianPhysModel(Vector2D pos, Vector2D vel, Vector2D accel);
	NewtonianPhysModel(Vector2D pos, Vector2D vel);
	NewtonianPhysModel(Vector2D pos);
	NewtonianPhysModel();

	// Getters, setters, and shifters

	const Vector2D& pos() const;
	const Vector2D& vel() const;
	const Vector2D& accel() const;
	const Vector2D& rot() const;
	const float& rotVel() const;
	const float& rotAccel() const;

	void setPos(const Vector2D& value);
	void setVel(const Vector2D& value);
	void setAccel(const Vector2D& value);
	void setRot(const Vector2D& value);
	void setRotVel(const float& value);
	void setRotAccel(const float& value);

	void shiftPos(const Vector2D& amount);
	void shiftVel(const Vector2D& amount);
	void shiftAccel(const Vector2D& amount);
	void shiftRot(const Vector2D& amount);
	void shiftRotVel(const float& amount);
	void shiftRotAccel(const float& amount);

	// Implement PhysModel

	virtual void run() override;
};
