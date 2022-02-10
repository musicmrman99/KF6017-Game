#include "Ship.h"

#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>

#define FPS 60
const float RPS = 2 * M_PI / FPS;

/* Lifecycle
-------------------------------------------------- */

Ship::Ship(Vector2D pos, Vector2D rot, PictureIndex image) {
    this->pos = pos;
    vel.set(0, 0);
    accel.set(0, 0);

    this->rot = rot;
    rotVel = 0.0f;

    engineThrust = 0.1;
    rotateThrust = 0.3 * RPS;

    this->image = image;
}

Ship::~Ship() {}

void Ship::beforeActions() {
    accel.set(0.0f, 0.0f);
    rotVel = 0.0f;
}

void Ship::runActions(std::vector<Action> actions) {
    for (Action action : actions) {
        switch (action) {
            case Ship::Action::MAIN_THRUST: mainThrust(); break;
            case Ship::Action::TURN_LEFT_THRUST: turnLeftThrust(); break;
            case Ship::Action::TURN_RIGHT_THRUST: turnRightThrust(); break;
        }
    }
}

void Ship::afterActions() {}

void Ship::physUpdate() {
    rot.setBearing(rot.angle() + rotVel, 1);

    vel += accel;
    pos += vel;
}

void Ship::draw() {
    MyDrawEngine* graphics = MyDrawEngine::GetInstance();
    graphics->DrawAt(pos, image, 1.0f, rot.angle(), 0.0f);
}

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
