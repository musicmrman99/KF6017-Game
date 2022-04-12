#pragma once

#include "EventEmitter.h"

class EventEmitterObserver {
private:
    EventEmitter::WPtr _eventEmitter;

public:
    using WPtr = std::weak_ptr<EventEmitterObserver>;

    virtual ~EventEmitterObserver() {}

    EventEmitter::WPtr eventEmitterWPtr() const;
    EventEmitter::Ptr eventEmitter() const;
    void updateEventEmitter(EventEmitter::WPtr eventEmitter);
};
