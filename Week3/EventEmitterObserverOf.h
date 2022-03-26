#pragma once

#include "Derived.h"
#include "ptrcast.h"

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
        return static_weak_pointer_cast<TDEventEmitter>(EventEmitterObserver::eventEmitterWPtr());
    }
    TDEventEmitter& eventEmitter() const { return *eventEmitterWPtr().lock(); }
};
