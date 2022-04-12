#include "HasEventHandler.h"

HasEventHandler::HasEventHandler(EventHandler::Ptr eventHandler) {
    setEventHandler(eventHandler);
}
HasEventHandler::~HasEventHandler() {}

void HasEventHandler::trackEventHandlerObserver(EventHandlerObserver::WPtr eventHandlerObserver) {
    if (auto po = eventHandlerObserver.lock()) {
        po->updateEventHandler(_eventHandler);
        _observerTracker.track(eventHandlerObserver);
    }
}

EventHandler::WPtr HasEventHandler::eventHandlerWPtr() const { return _eventHandler; }
EventHandler& HasEventHandler::eventHandler() const { return *_eventHandler; }

void HasEventHandler::setEventHandler(EventHandler::Ptr eventHandler) {
    // Set the model
    if (eventHandler) _eventHandler = eventHandler;
    else _eventHandler = EventHandler::Ptr(new NullEventHandler()); // Do not allow nullptr

    // Update all observers
    bool areAnyExpired = false;
    for (EventHandlerObserver::WPtr& observer : _observerTracker.getTracked()) {
        if (auto ob = observer.lock()) {
            ob->updateEventHandler(eventHandler);
        } else {
            areAnyExpired = true;
        }
    }
    if (areAnyExpired) _observerTracker.dropExpired();
}

void HasEventHandler::handle(const Event::Ptr e) {
    _eventHandler->handle(e);
}
