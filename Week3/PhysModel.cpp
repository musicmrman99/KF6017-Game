#include "PhysModel.h"
#include "Game.h"

/* Phys Model
-------------------------------------------------- */

PhysModel::~PhysModel() {}

// Game Units -> Raw Units

const Vector2D PhysModel::fromDUPS(const Vector2D distanceUnitsPerSecond) const {
	return distanceUnitsPerSecond * (DIST_UNIT * (float) Game::gt.mdFrameTime);
}
const float PhysModel::fromDUPS(const float distanceUnitsPerSecond) const {
	return distanceUnitsPerSecond * (DIST_UNIT * (float) Game::gt.mdFrameTime);
}
const float PhysModel::fromRPS(const float rotationPerSecond) const {
	return rotationPerSecond * (TWO_PI * (float) Game::gt.mdFrameTime);
}

Vector2D PhysModel::fromDUPS(Vector2D distanceUnitsPerSecond) {
	return distanceUnitsPerSecond * (DIST_UNIT * (float) Game::gt.mdFrameTime);
}
float PhysModel::fromDUPS(float distanceUnitsPerSecond) {
	return distanceUnitsPerSecond * (DIST_UNIT * (float) Game::gt.mdFrameTime);
}
float PhysModel::fromRPS(float rotationPerSecond) {
	return rotationPerSecond * (TWO_PI * (float) Game::gt.mdFrameTime);
}

// Raw Units -> Game Units

const Vector2D PhysModel::toDUPS(const Vector2D rawDistanceUnitsPerSecond) const {
	return rawDistanceUnitsPerSecond / (DIST_UNIT * (float) Game::gt.mdFrameTime);
}
const float PhysModel::toDUPS(const float rawDistanceUnitsPerSecond) const {
	return rawDistanceUnitsPerSecond / (DIST_UNIT * (float) Game::gt.mdFrameTime);
}
const float PhysModel::toRPS(const float rawRotationPerSecond) const {
	return rawRotationPerSecond / (TWO_PI * (float) Game::gt.mdFrameTime);
}

Vector2D PhysModel::toDUPS(Vector2D rawDistanceUnitsPerSecond) {
	return rawDistanceUnitsPerSecond / (DIST_UNIT * (float) Game::gt.mdFrameTime);
}
float PhysModel::toDUPS(float rawDistanceUnitsPerSecond) {
	return rawDistanceUnitsPerSecond / (DIST_UNIT * (float) Game::gt.mdFrameTime);
}
float PhysModel::toRPS(float rawRotationPerSecond) {
	return rawRotationPerSecond / (TWO_PI * (float) Game::gt.mdFrameTime);
}

/* Null Phys Model
-------------------------------------------------- */

// Do nothing.
void NullPhysModel::run() {}
