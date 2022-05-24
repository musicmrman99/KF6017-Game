#include "BasicMovement.h"

// Creation

BasicMovement::BasicMovement(float posThrust, float rotThrust) :
    _positionalThrust(posThrust),  // Distance units / second^2
    _rotationalThrust(rotThrust) // Revolutions / second^2
{}
BasicMovement::UPtr BasicMovement::create(float posThrust, float rotThrust) {
    return UPtr(new BasicMovement(posThrust, rotThrust));
}

// Getters

float BasicMovement::positionalThrust() { return _positionalThrust; }
float BasicMovement::rotationalThrust() { return _rotationalThrust; }

// Events

const EventType BasicMovement::MainThrustEvent::TYPE;
BasicMovement::MainThrustEvent::MainThrustEvent() : Event(TYPE) {}
void BasicMovement::MainThrustEventEmitter::emit(std::queue<Event::Ptr>& events) {
    events.push(MainThrustEvent::Ptr(new MainThrustEvent()));
}

const EventType BasicMovement::TurnLeftThrustEvent::TYPE;
BasicMovement::TurnLeftThrustEvent::TurnLeftThrustEvent() : Event(TYPE) {}
void BasicMovement::TurnLeftThrustEventEmitter::emit(std::queue<Event::Ptr>& events) {
    events.push(TurnLeftThrustEvent::Ptr(new TurnLeftThrustEvent()));
}

const EventType BasicMovement::TurnRightThrustEvent::TYPE;
BasicMovement::TurnRightThrustEvent::TurnRightThrustEvent() : Event(TYPE) {}
void BasicMovement::TurnRightThrustEventEmitter::emit(std::queue<Event::Ptr>& events) {
    events.push(TurnRightThrustEvent::Ptr(new TurnRightThrustEvent()));
}

// Actions

void BasicMovement::mainThrust() {
    physModel().shiftAccel(physModel().rot() * _positionalThrust);
};

void BasicMovement::turnLeftThrust() {
    physModel().shiftRotAccel(-_rotationalThrust);
};

void BasicMovement::turnRightThrust() {
    physModel().shiftRotAccel(_rotationalThrust);
}

// Handler

void BasicMovement::handle(const Event::Ptr e) {
    if (e->type == MainThrustEvent::TYPE) mainThrust();
    else if (e->type == TurnLeftThrustEvent::TYPE) turnLeftThrust();
    else if (e->type == TurnRightThrustEvent::TYPE) turnRightThrust();
}
