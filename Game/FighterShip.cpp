#include "FighterShip.h"

#include <string>

#include "ptrcast.h"

#include "BulletSpec.h"

/* Event Handler
-------------------------------------------------- */

/* Initialisation
-------------------- */

FighterShipEventHandler::FighterShipEventHandler(FighterShipSpec::Ptr spec)
    : bulletImage(spec->bulletImage),
    engineThrust(0.1f),  // Distance units / second^2
    rotateThrust(0.008f) // Revolutions / second^2
{}

/* Dispatcher
-------------------- */

void FighterShipEventHandler::handle(const Event::Ptr e) {
    if (e->type == MainThrustEvent::TYPE) mainThrust();
    else if (e->type == TurnLeftThrustEvent::TYPE) turnLeftThrust();
    else if (e->type == TurnRightThrustEvent::TYPE) turnRightThrust();

    else if (e->type == FireEvent::TYPE) fire();
}

/* Movement
-------------------- */

// Events

const EventType FighterShipEventHandler::MainThrustEvent::TYPE;
FighterShipEventHandler::MainThrustEvent::MainThrustEvent() : Event(TYPE) {}
void FighterShipEventHandler::MainThrustEventEmitter::emit(std::queue<Event::Ptr>& events) {
    events.push(MainThrustEvent::Ptr(new MainThrustEvent()));
}

const EventType FighterShipEventHandler::TurnLeftThrustEvent::TYPE;
FighterShipEventHandler::TurnLeftThrustEvent::TurnLeftThrustEvent() : Event(TYPE) {}
void FighterShipEventHandler::TurnLeftThrustEventEmitter::emit(std::queue<Event::Ptr>& events) {
    events.push(TurnLeftThrustEvent::Ptr(new TurnLeftThrustEvent()));
}

const EventType FighterShipEventHandler::TurnRightThrustEvent::TYPE;
FighterShipEventHandler::TurnRightThrustEvent::TurnRightThrustEvent() : Event(TYPE) {}
void FighterShipEventHandler::TurnRightThrustEventEmitter::emit(std::queue<Event::Ptr>& events) {
    events.push(TurnRightThrustEvent::Ptr(new TurnRightThrustEvent()));
}

// Actions

void FighterShipEventHandler::mainThrust() {
    physModel().shiftAccel(physModel().rot() * engineThrust);
};

void FighterShipEventHandler::turnLeftThrust() {
    physModel().shiftRotAccel(-rotateThrust);
};

void FighterShipEventHandler::turnRightThrust() {
    physModel().shiftRotAccel(rotateThrust);
};

/* Attack
-------------------- */

// Event

const EventType FighterShipEventHandler::FireEvent::TYPE;
FighterShipEventHandler::FireEvent::FireEvent() : Event(TYPE) {}
void FighterShipEventHandler::FireEventEmitter::emit(std::queue<Event::Ptr>& events) {
    events.push(FireEvent::Ptr(new FireEvent()));
}

// Action

void FighterShipEventHandler::fire() {
    static constexpr float DIST = 40.0f;
    eventEmitter().enqueue(objectEventFactory()->createObject(
        BulletSpec::UPtr(new BulletSpec(
            physModel().pos() + DIST * physModel().rot(),
            physModel().rot(),
            physModel().vel(),
            bulletImage
        ))
    ));
}

/* FighterShip
-------------------------------------------------- */

FighterShip::FighterShip(FighterShipSpec::Ptr spec) :
    Ship(spec),
    HasEventHandlerOf(FighterShipEventHandler::UPtr(new FighterShipEventHandler(spec)))
{
    // Thread dependencies
    trackPhysObserver(eventHandlerWPtr());
    trackEventEmitterObserver(eventHandlerWPtr());
}

const ObjectFactory FighterShip::factory = [](ObjectSpec::UPtr spec) {
    return GameObject::Ptr(new FighterShip(static_unique_pointer_cast<FighterShipSpec>(move(spec))));
};

void FighterShip::setObjectEventFactory(ObjectEventFactory::Ptr objectEventFactory) {
    ObjectEventCreator::setObjectEventFactory(objectEventFactory);
    eventHandler().setObjectEventFactory(objectEventFactory);
}
