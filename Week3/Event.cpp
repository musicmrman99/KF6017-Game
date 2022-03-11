#include "Event.h"

/* Core Types
-------------------------------------------------- */

// Event
Event::Event() {}
Event::Event(const Event::Ptr& ptr) {}
Event::~Event() {}

// Event Emitter
EventEmitter::~EventEmitter() {}

// Event Handler
EventHandler::~EventHandler() {}

// Targetted Event
TargettedEvent::TargettedEvent(Event::Ptr event, const EventHandler::WPtr target)
	: event(event), target(target) {
}
TargettedEvent::~TargettedEvent() {}

/* Null Types
-------------------------------------------------- */

// Null Event Emitter
void NullEventEmitter::emit(std::queue<Event::Ptr>& events) {}

// Null Event Handler
void NullEventHandler::handle(const Event::Ptr e) {}
