#include "Ship.h"

#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>

#define FPS 60
const float RPS = 2 * M_PI / FPS;

/* Actions
-------------------------------------------------- */

void Ship::mainThrust() {
    accel += rot * engineThrust;
}

void Ship::turnLeftThrust() {
    rotVel -= rotateThrust;
}

void Ship::turnRightThrust() {
    rotVel += rotateThrust;
}

void Ship::setActionSource(ActionSource<Action>* actionSource) {
    this->actionSource = actionSource;
}

/* Lifecycle
-------------------------------------------------- */

Ship::Ship(Vector2D pos, Vector2D rot, PictureIndex image)
    : pos(pos), rot(rot), image(image), actionSource(nullptr)
{
    vel.set(0, 0);
    accel.set(0, 0);

    rotVel = 0.0f;

    engineThrust = 0.1;
    rotateThrust = 0.3 * RPS;
}

Ship::~Ship() {}

void Ship::beforeActions() {
    accel.set(0.0f, 0.0f);
    rotVel = 0.0f;
}
void Ship::actions() {
    for (Action action : actionSource->getActions()) {
        switch (action) {
            case Ship::Action::MAIN_THRUST: mainThrust(); break;
            case Ship::Action::TURN_LEFT_THRUST: turnLeftThrust(); break;
            case Ship::Action::TURN_RIGHT_THRUST: turnRightThrust(); break;
        }
    }
}

void Ship::beforePhys() {}
void Ship::phys() {
    rot.setBearing(rot.angle() + rotVel, 1);

    vel += accel;
    pos += vel;
}

void Ship::beforeDraw() {}
void Ship::draw() {
    MyDrawEngine* graphics = MyDrawEngine::GetInstance();
    graphics->DrawAt(pos, image, 1.0f, rot.angle(), 0.0f);
}

void Ship::afterFrame() {}
