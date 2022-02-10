#pragma once

#include <vector>

#include "mydrawengine.h"
#include "myinputs.h"

class Ship {
private:
	PictureIndex image;

	// Object Quantities
	Vector2D pos;
	Vector2D vel;
	Vector2D accel;

	Vector2D rot;
	float rotVel;

	// Game Quantities
	float engineThrust;
	float rotateThrust;

public:
	// Game Actions
	enum class Action {
		MAIN_THRUST,
		TURN_LEFT_THRUST,
		TURN_RIGHT_THRUST
	};

	void mainThrust();
	void turnLeftThrust();
	void turnRightThrust();

	// Lifecycle
	Ship(Vector2D pos, Vector2D rot, PictureIndex image);
	~Ship();

	void beforeActions();
	void runActions(std::vector<Action> actions);
	void afterActions();
	void physUpdate();
	void draw();
};
