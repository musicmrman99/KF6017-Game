#pragma once

#include <vector>

#include "mydrawengine.h"
#include "myinputs.h"

#include "ActionSource.h"

class Ship {
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

private:
	ActionSource<Action>* actionSource;

	// Graphical Representation
	PictureIndex image;

	// Logical Representation
	Vector2D pos;
	Vector2D vel;
	Vector2D accel;

	Vector2D rot;
	float rotVel;

	// Gameplay Quantities
	float engineThrust;
	float rotateThrust;

public:
	void setActionSource(ActionSource<Action>* actionSource);

	// Lifecycle
	Ship(Vector2D pos, Vector2D rot, PictureIndex image);
	~Ship();

	void beforeActions();
	void actions();

	void beforePhys();
	void phys();

	void beforeDraw();
	void draw();

	void afterFrame();
};
