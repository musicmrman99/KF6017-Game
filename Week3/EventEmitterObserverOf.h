#pragma once

#include "Derived.h"

#include "EventEmitterObserver.h"

// Trait of objects that depend on reading an EventEmitter.
template <class TEventEmitter>
class EventEmitterObserverOf : public EventEmitterObserver {
private:
    // A (T)ype (D)erived from (EventEmitter)
    using TDEventEmitter = Derived<TEventEmitter, EventEmitter>;
    using TDEventEmitterPtr = std::shared_ptr<TDEventEmitter>;
    using TDEventEmitterWPtr = std::weak_ptr<TDEventEmitter>;

public:
    using WPtr = std::weak_ptr<EventEmitterObserverOf<TDEventEmitter>>;

    const TDEventEmitterWPtr eventEmitterWPtr() const {
        return std::static_pointer_cast<TDEventEmitter>(EventEmitterObserver::eventEmitter());
    }
    const TDEventEmitterPtr eventEmitter() const { return eventEmitterWPtr().lock(); }
};
