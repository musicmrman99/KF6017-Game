#pragma once

#include <memory>

#include "Tracker.h"

#include "EventEmitter.h"
#include "EventEmitterObserver.h"

/*
GameObject Trait: Adds the Event Handling component slot.

This includes:
- The EventEmitter (use eventEmitter()/setEventEmitter() for get/set)
- An observer tracker for the EventEmitter dependants (use trackEventEmitterObserver() to add a dependant object)
- The Event Handling lifecycle method (handle())
*/
class HasEventEmitter : public EventEmitter {
private:
    Tracker<EventEmitterObserver> _observerTracker;
    EventEmitter::Ptr _eventEmitter;

public:
    using Ptr = std::shared_ptr<HasEventEmitter>;
    using UPtr = std::unique_ptr<HasEventEmitter>;
    using WPtr = std::weak_ptr<HasEventEmitter>;

    // Initialise the Event Handling component slot
    HasEventEmitter(EventEmitter::Ptr eventEmitter);

    virtual ~HasEventEmitter();

    // Make changing the EventEmitter reflected in the given EventEmitter-dependant object.
    void trackEventEmitterObserver(EventEmitterObserver::WPtr eventEmitterObserver);

    EventEmitter::WPtr eventEmitterWPtr() const;          // Useful for setting the EventEmitter as an observer of another model
    EventEmitter& eventEmitter() const;                   // Get the EventEmitter
    void setEventEmitter(EventEmitter::Ptr eventEmitter); // Set the EventEmitter (and update all observers)

    // Run the "emit" lifecycle method
    // Note that HasEventEmitter also IS an EventEmitter (ie. definition is recursive, but not branching)
    virtual void emit(std::queue<Event::Ptr>& events) override;
};
