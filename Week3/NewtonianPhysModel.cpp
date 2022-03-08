#include "NewtonianPhysModel.h"

// Constructors

NewtonianPhysModel::NewtonianPhysModel(Vector2D pos, Vector2D vel, Vector2D accel, Vector2D rot, float rotVel, float rotAccel)
    : _pos(pos), _vel(vel), _accel(accel), _rot(rot), _rotVel(rotVel), _rotAccel(rotAccel) {}

NewtonianPhysModel::NewtonianPhysModel(Vector2D pos, Vector2D vel, Vector2D rot, float rotVel)
    : NewtonianPhysModel(pos, vel, Vector2D(0.0f, 0.0f), rot, rotVel, 0.0f) {}

NewtonianPhysModel::NewtonianPhysModel(Vector2D pos, Vector2D vel, Vector2D accel)
    : NewtonianPhysModel(pos, vel, accel, Vector2D(0.0f, 1.0f), 0.0f, 0.0f) {}

NewtonianPhysModel::NewtonianPhysModel(Vector2D pos, Vector2D vel)
    : NewtonianPhysModel(pos, vel, Vector2D(0.0f, 0.0f)) {}

NewtonianPhysModel::NewtonianPhysModel(Vector2D pos)
    : NewtonianPhysModel(pos, Vector2D(0.0f, 0.0f)) {}

NewtonianPhysModel::NewtonianPhysModel()
    : NewtonianPhysModel(Vector2D(0.0f, 0.0f)) {}

// Getters, setters, and shifters

const Vector2D& NewtonianPhysModel::pos() const { return _pos; }
const Vector2D NewtonianPhysModel::vel() const { return toDUPS(_vel); }
const Vector2D NewtonianPhysModel::accel() const { return toDUPS(_accel); }
const Vector2D& NewtonianPhysModel::rot() const { return _rot; }
const float NewtonianPhysModel::rotVel() const { return toRPS(_rotVel); }
const float NewtonianPhysModel::rotAccel() const { return toRPS(_rotAccel); }

void NewtonianPhysModel::setPos(const Vector2D& value) { _pos = value; }
void NewtonianPhysModel::setVel(const Vector2D& value) { _vel = fromDUPS(value); }
void NewtonianPhysModel::setAccel(const Vector2D& value) { _accel = fromDUPS(value); }
void NewtonianPhysModel::setRot(const Vector2D& value) { _rot = value; }
void NewtonianPhysModel::setRotVel(const float& value) { _rotVel = fromRPS(value); }
void NewtonianPhysModel::setRotAccel(const float& value) { _rotAccel = fromRPS(value); }

void NewtonianPhysModel::shiftPos(const Vector2D& value) { _pos += value; }
void NewtonianPhysModel::shiftVel(const Vector2D& value) { _vel += fromDUPS(value); }
void NewtonianPhysModel::shiftAccel(const Vector2D& value) { _accel += fromDUPS(value); }
void NewtonianPhysModel::shiftRot(const Vector2D& value) { _rot += value; }
void NewtonianPhysModel::shiftRotVel(const float& value) { _rotVel += fromRPS(value); }
void NewtonianPhysModel::shiftRotAccel(const float& value) { _rotAccel += fromRPS(value); }

// Implement PhysModel

void NewtonianPhysModel::run() {
    // It's faster to store it in raw units and convert as needed because
    // running the model must be done every frame, whereas get/set/shift may not be.

    _rotVel += _rotAccel;
    _rot.setBearing(_rot.angle() + _rotVel, 1);

    _vel += _accel;
    _pos += _vel;
}
