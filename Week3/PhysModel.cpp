#include "PhysModel.h"
#include "Game.h"

/* Phys Model
-------------------------------------------------- */

PhysModel::~PhysModel() {}

float PhysModel::toDUPS(float mpf) {
	return mpf * DIST_UNIT * (float) Game::gt.mdFrameTime;
}

float PhysModel::toRPS(float rpf) {
	return rpf * TWO_PI * (float) Game::gt.mdFrameTime;
}

/* Null Phys Model
-------------------------------------------------- */

// Do nothing.
void NullPhysModel::run() {}
