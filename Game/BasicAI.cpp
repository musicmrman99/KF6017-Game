#include "BasicAI.h"

#include "QueueUtils.h"
#include "DropExpiredFrom.h"

#include "HasEventHandler.h"
#include "TargettedEvent.h"

#include "TargettingStrategy.h"

#include "mydrawengine.h"

BasicAI::BasicAI(
    const TargettingStrategy::Ptr targettingStrategy,
    const MovementStrategy::Ptr movementStrategy,
    const AttackStrategy::Ptr attackStrategy
) :
    _targettingStrategy(targettingStrategy),
    _movementStrategy(movementStrategy),
    _attackStrategy(attackStrategy)
{}
BasicAI::UPtr BasicAI::create(
    const TargettingStrategy::Ptr targettingStrategy,
    const MovementStrategy::Ptr movementStrategy,
    const AttackStrategy::Ptr attackStrategy
) {
    return UPtr(new BasicAI(
        targettingStrategy,
        movementStrategy,
        attackStrategy
    ));
}

void BasicAI::add(
    HasPhysOf<NewtonianPhysModel>::WPtr object,
    TargettingData::Ptr targettingData,
    MovementData::Ptr movementData,
    AttackData::Ptr attackData
) {
    controlledObjects.push_back(ControlledObject {
        object,
        targettingData,
        movementData,
        attackData
    });
}

TargettingStrategy::Ptr BasicAI::targettingStrategy() const { return _targettingStrategy; }
MovementStrategy::Ptr BasicAI::movementStrategy() const { return _movementStrategy; }
AttackStrategy::Ptr BasicAI::attackStrategy() const { return _attackStrategy; }

void BasicAI::emit(std::queue<Event::Ptr>& events) {
    static std::queue<Event::Ptr> eventsBuffer;

    // Remove destroyed controlled and target objects. At some point, this may
    // be useful information for the AI, but it isn't for now.
    dropExiredFrom(controlledObjects);

    // Control objects and update AI data as needed
    _targettingStrategy->selectTargets(controlledObjects);
    _movementStrategy->moveObjects(controlledObjects, eventsBuffer);
    _attackStrategy->attack(controlledObjects, eventsBuffer);

    // Flush the buffer
    shiftInto(eventsBuffer, events);
}
