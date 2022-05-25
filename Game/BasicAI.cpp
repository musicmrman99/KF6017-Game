#include "BasicAI.h"

#include "QueueUtils.h"
#include "DropExpiredFrom.h"

#include "HasEventHandler.h"
#include "TargettedEvent.h"

#include "TargettingStrategy.h"

#include "mydrawengine.h"

BasicAI::BasicAI(
    const TargettingStrategy::Ptr targettingStrategy,
    const MovementStrategy::Ptr movementStrategy
) :
    _targettingStrategy(targettingStrategy),
    _movementStrategy(movementStrategy)
{}
BasicAI::UPtr BasicAI::create(
    const TargettingStrategy::Ptr targettingStrategy,
    const MovementStrategy::Ptr movementStrategy
) {
    return UPtr(new BasicAI(
        targettingStrategy,
        movementStrategy
    ));
}

void BasicAI::add(
        HasPhysOf<NewtonianPhysModel>::WPtr object,
        TargettingData::Ptr targettingData
) {
    controlledObjects.push_back(ControlledObject {
        object,
        targettingData
    });
}

TargettingStrategy::Ptr BasicAI::targettingStrategy() const { return _targettingStrategy; }
MovementStrategy::Ptr BasicAI::movementStrategy() const { return _movementStrategy; }

void BasicAI::emit(std::queue<Event::Ptr>& events) {
    static std::queue<Event::Ptr> eventsBuffer;

    // Remove destroyed controlled and target objects. At some point, this may
    // be useful information for the AI, but it isn't for now.
    dropExiredFrom(controlledObjects);

    // 1. Avoid collidable targets
    // 2. Stay ~N distance away from all targets

    // 3. Select a target / Should switch target?
    _targettingStrategy->selectTargets(controlledObjects);

    // 4. Try to face targets (so you can fire at them)
    _movementStrategy->moveObjects(controlledObjects, eventsBuffer);
    
    // 5. Fire at targets
    // 

    // 6. Avoid other collidable objects

    /*
    TargettedEvent::Ptr(new TargettedEvent(
        EventEmitter->emit(eventsBuffer),
        object
    ))
    */

    // Flush the buffer
    shiftInto(eventsBuffer, events);
}
