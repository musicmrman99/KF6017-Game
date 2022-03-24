#pragma once

#include "EventHandler.h"

class EventHandlerObserver {
private:
    EventHandler::WPtr _eventHandler;

public:
    using WPtr = std::weak_ptr<EventHandlerObserver>;

    virtual ~EventHandlerObserver() {}

    EventHandler::WPtr eventHandlerWPtr() const;
    EventHandler::Ptr eventHandler() const;
    void updateEventHandler(EventHandler::WPtr eventHandler);
};
