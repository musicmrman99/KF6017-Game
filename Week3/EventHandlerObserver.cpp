#include "EventHandlerObserver.h"

EventHandler::WPtr EventHandlerObserver::eventHandlerWPtr() const { return _eventHandler; }
EventHandler::Ptr EventHandlerObserver::eventHandler() const { return eventHandlerWPtr().lock(); }
void EventHandlerObserver::updateEventHandler(EventHandler::WPtr eventHandler) { _eventHandler = eventHandler; }
