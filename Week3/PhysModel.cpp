#include "PhysModel.h"

#include "mydrawengine.h"

GameTimer PhysModel::gt;

PhysModel::~PhysModel() {}

float PhysModel::toDUPS(float mpf) {
	return mpf * DIST_UNIT * (float) gt.mdFrameTime;
}

float PhysModel::toRPS(float rpf) {
	return rpf * TWO_PI * (float) gt.mdFrameTime;
}
