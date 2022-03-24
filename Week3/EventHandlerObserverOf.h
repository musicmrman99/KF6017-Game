#pragma once

#include "Derived.h"
#include "ptrcast.h"

#include "EventHandlerObserver.h"

// Trait of objects that depend on reading an EventHandler.
template <class TEventHandler>
class EventHandlerObserverOf : public EventHandlerObserver {
private:
    // A (T)ype (D)erived from (EventHandler)
    using TDEventHandler = Derived<TEventHandler, EventHandler>;
    using TDEventHandlerPtr = std::shared_ptr<TDEventHandler>;
    using TDEventHandlerWPtr = std::weak_ptr<TDEventHandler>;

public:
    using WPtr = std::weak_ptr<EventHandlerObserverOf<TDEventHandler>>;

    const TDEventHandlerWPtr eventHandlerWPtr() const {
        return static_weak_pointer_cast<TDEventHandler>(EventHandlerObserver::eventHandler());
    }
    const TDEventHandlerPtr eventHandler() const { return eventHandlerWPtr().lock(); }
};
