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
const Vector2D& NewtonianPhysModel::vel() const { return _vel; }
const Vector2D& NewtonianPhysModel::accel() const { return _accel; }
const Vector2D& NewtonianPhysModel::rot() const { return _rot; }
const float& NewtonianPhysModel::rotVel() const { return _rotVel; }
const float& NewtonianPhysModel::rotAccel() const { return _rotAccel; }

void NewtonianPhysModel::setPos(const Vector2D& value) { _pos = value; }
void NewtonianPhysModel::setVel(const Vector2D& value) { _vel = value; }
void NewtonianPhysModel::setAccel(const Vector2D& value) { _accel = value; }
void NewtonianPhysModel::setRot(const Vector2D& value) { _rot = value; }
void NewtonianPhysModel::setRotVel(const float& value) { _rotVel = value; }
void NewtonianPhysModel::setRotAccel(const float& value) { _rotAccel = value; }

void NewtonianPhysModel::shiftPos(const Vector2D& value) { _pos += value; }
void NewtonianPhysModel::shiftVel(const Vector2D& value) { _vel += value; }
void NewtonianPhysModel::shiftAccel(const Vector2D& value) { _accel += value; }
void NewtonianPhysModel::shiftRot(const Vector2D& value) { _rot += value; }
void NewtonianPhysModel::shiftRotVel(const float& value) { _rotVel += value; }
void NewtonianPhysModel::shiftRotAccel(const float& value) { _rotAccel += value; }

// Implement PhysModel

void NewtonianPhysModel::run() {
    _rotVel += _rotAccel;
    _rot.setBearing(_rot.angle() + _rotVel, 1);

    _vel += _accel;
    _pos += _vel;
}
