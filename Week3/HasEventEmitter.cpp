#include "HasEventEmitter.h"

HasEventEmitter::HasEventEmitter(EventEmitter::Ptr eventEmitter) {
    setEventEmitter(eventEmitter);
}
HasEventEmitter::~HasEventEmitter() {}

void HasEventEmitter::trackEventEmitterObserver(EventEmitterObserver::WPtr eventEmitterObserver) {
    if (auto po = eventEmitterObserver.lock()) {
        po->updateEventEmitter(_eventEmitter);
        _observerTracker.track(eventEmitterObserver);
    }
}

EventEmitter::WPtr HasEventEmitter::eventEmitterWPtr() const { return _eventEmitter; }
EventEmitter& HasEventEmitter::eventEmitter() const { return *_eventEmitter; }

void HasEventEmitter::setEventEmitter(EventEmitter::Ptr eventEmitter) {
    // Set the model
    if (eventEmitter) _eventEmitter = eventEmitter;
    else _eventEmitter = EventEmitter::Ptr(new NullEventEmitter()); // Do not allow nullptr

    // Update all observers
    bool areAnyExpired = false;
    for (EventEmitterObserver::WPtr& observer : _observerTracker.getTracked()) {
        if (auto ob = observer.lock()) {
            ob->updateEventEmitter(eventEmitter);
        } else {
            areAnyExpired = true;
        }
    }
    if (areAnyExpired) _observerTracker.dropExpired();
}

void HasEventEmitter::emit(std::queue<Event::Ptr>& events) {
    _eventEmitter->emit(events);
}
