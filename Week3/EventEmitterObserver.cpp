#include "EventEmitterObserver.h"

EventEmitter::WPtr EventEmitterObserver::eventEmitterWPtr() const { return _eventEmitter; }
EventEmitter::Ptr EventEmitterObserver::eventEmitter() const { return eventEmitterWPtr().lock(); }
void EventEmitterObserver::updateEventEmitter(EventEmitter::WPtr eventEmitter) { _eventEmitter = eventEmitter; }
