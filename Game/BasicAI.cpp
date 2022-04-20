#include "BasicAI.h"

#include "QueueUtils.h"

#include "HasEventHandler.h"
#include "TargettedEvent.h"

#include "TargettingStrategy.h"

#include "mydrawengine.h"

BasicAI::BasicAI(
    const TargettingStrategy::Ptr targettingStrategy,
    const MovementStrategy::Ptr movementStrategy
) :
    targettingStrategy(targettingStrategy),
    movementStrategy(movementStrategy)
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

void BasicAI::addControlledObject(HasPhysOf<NewtonianPhysModel>::WPtr object) {
    // Track
    controlledObjects.push_back(ControlledObject { object, TargetObject {} });
}

void BasicAI::addTarget(HasPhysOf<NewtonianPhysModel>::WPtr object) {
    // Track
    targetCandidates.push_back(TargetObject { object });
}

void BasicAI::emit(std::queue<Event::Ptr>& events) {
    static std::queue<Event::Ptr> eventsBuffer;

    // Remove destroyed controlled and target objects. At some point, this may
    // be useful information for the AI, but it isn't for now.
    dropExiredFrom(controlledObjects);
    dropExiredFrom(targetCandidates);

    // 1. Avoid collidable targets
    // 2. Stay ~N distance away from all targets

    // 3. Select a target / Should switch target?
    targettingStrategy->selectTargets(
        controlledObjects,
        targetCandidates
    );

    // 4. Try to face targets (so you can fire at them)
    movementStrategy->moveObjects(
        controlledObjects,
        eventsBuffer
    );
    
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
