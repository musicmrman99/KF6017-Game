#pragma once

#include <memory>

#include "Tracker.h"

#include "EventHandler.h"
#include "EventHandlerObserver.h"

/*
GameObject Trait: Adds the Event Handling component slot.

This includes:
- The EventHandler (use eventHandler()/setEventHandler() for get/set)
- An observer tracker for the EventHandler dependants (use trackEventHandlerObserver() to add a dependant object)
- The Event Handling lifecycle method (handle())
*/
class HasEventHandler : public EventHandler {
private:
    Tracker<EventHandlerObserver> _observerTracker;
    EventHandler::Ptr _eventHandler;

public:
    using Ptr = std::shared_ptr<HasEventHandler>;
    using UPtr = std::unique_ptr<HasEventHandler>;
    using WPtr = std::weak_ptr<HasEventHandler>;

    // Initialise the Event Handling component slot
    HasEventHandler(EventHandler::Ptr eventHandler);

    virtual ~HasEventHandler();

    // Make changing the EventHandler reflected in the given EventHandler-dependant object.
    void trackEventHandlerObserver(EventHandlerObserver::WPtr eventHandlerObserver);

    EventHandler::WPtr eventHandlerWPtr() const;          // Useful for setting the EventHandler as an observer of another model
    EventHandler& eventHandler() const;                   // Get the EventHandler
    void setEventHandler(EventHandler::Ptr eventHandler); // Set the EventHandler (and update all observers)

    // Run the "emit" lifecycle method
    // Note that HasEventHandler also IS an EventHandler (ie. definition is recursive, but not branching)
    virtual void handle(const Event::Ptr e) override;
};
