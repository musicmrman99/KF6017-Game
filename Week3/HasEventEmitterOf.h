#pragma once

#include "Derived.h"
#include "ptrcast.h"

#include "HasEventEmitter.h"
#include "EventEmitterObserverOf.h"

// GameObject Trait: Adds the Event Emitter component slot and restricts it to holding a particular
// Event Emitter type.
// 
// Warning: Using objects of this type as a HasEventEmitter to set the EventEmitter to something not derived
//          from TEventEmitter, then using these functions to get it as a TEventEmitter is equivalent to
//          performing an ordinary dangerous static_cast, and is therefore liable to cause memory
//          corruption and/or access violations.
template <class TEventEmitter>
class HasEventEmitterOf : public HasEventEmitter {
private:
    // A (T)ype (D)erived from (EventEmitter)
    using TDEventEmitter = Derived<TEventEmitter, EventEmitter>;
    using TDEventEmitterPtr = std::shared_ptr<TDEventEmitter>;
    using TDEventEmitterWPtr = std::weak_ptr<TDEventEmitter>;

public:
    HasEventEmitterOf(TDEventEmitterPtr eventEmitter) : HasEventEmitter(eventEmitter) {}

    // Note: Non-virtual hiding is intended.

    // Make changing the EventEmitter reflected in the given EventEmitter-dependant object.
    void trackEventEmitterObserver(typename EventEmitterObserverOf<TDEventEmitter>::WPtr eventEmitterObserver) {
        HasEventEmitter::trackEventEmitterObserver(eventEmitterObserver);
    }

    // Useful for setting the EventEmitter as an observer of another model
    TDEventEmitterWPtr eventEmitterWPtr() const { return static_weak_pointer_cast<TDEventEmitter>(HasEventEmitter::eventEmitterWPtr()); }
    TDEventEmitter& eventEmitter() const { return static_cast<TDEventEmitter&>(HasEventEmitter::eventEmitter()); }
    void setEventEmitter(TDEventEmitterPtr eventEmitter) {
        if (eventEmitter) HasEventEmitter::setEventEmitter(eventEmitter);
    }
};
