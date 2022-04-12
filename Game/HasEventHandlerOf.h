#pragma once

#include "Derived.h"
#include "ptrcast.h"

#include "HasEventHandler.h"
#include "EventHandlerObserverOf.h"

// GameObject Trait: Adds the Event Handler component slot and restricts it to holding a particular
// Event Handler type.
// 
// Warning: Using objects of this type as a HasEventHandler to set the EventHandler to something not derived
//          from TEventHandler, then using these functions to get it as a TEventHandler is equivalent to
//          performing an ordinary dangerous static_cast, and is therefore liable to cause memory
//          corruption and/or access violations.
template <class TEventHandler>
class HasEventHandlerOf : public HasEventHandler {
private:
    // A (T)ype (D)erived from (EventHandler)
    using TDEventHandler = Derived<TEventHandler, EventHandler>;
    using TDEventHandlerPtr = std::shared_ptr<TDEventHandler>;
    using TDEventHandlerWPtr = std::weak_ptr<TDEventHandler>;

public:
    HasEventHandlerOf(TDEventHandlerPtr eventHandler) : HasEventHandler(eventHandler) {}

    // Note: Non-virtual hiding is intended.

    // Make changing the EventHandler reflected in the given EventHandler-dependant object.
    void trackEventHandlerObserver(typename EventHandlerObserverOf<TDEventHandler>::WPtr eventHandlerObserver) { HasEventHandler::trackEventHandlerObserver(eventHandlerObserver); }

    // Useful for setting the EventHandler as an observer of another model
    TDEventHandlerWPtr eventHandlerWPtr() const { return static_weak_pointer_cast<TDEventHandler>(HasEventHandler::eventHandlerWPtr()); }
    TDEventHandler& eventHandler() const { return static_cast<TDEventHandler&>(HasEventHandler::eventHandler()); }
    void setEventHandler(TDEventHandlerPtr eventHandler) {
        if (eventHandler) HasEventHandler::setEventHandler(eventHandler);
    }
};
