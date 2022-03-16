#include "Event.h"

/* Core Types
-------------------------------------------------- */

// Event
Event::Event(const EventType& type) : type(type) {}
Event::Event(const Event::Ptr& ptr) : type(ptr->type) {}
Event::~Event() {}

// Event Emitter
EventEmitter::~EventEmitter() {}

// Event Handler
EventHandler::~EventHandler() {}

// Targetted Event
const EventType TargettedEvent::TYPE;
TargettedEvent::TargettedEvent(Event::Ptr event, const EventHandler::WPtr target)
	: Event(TYPE), event(event), target(target) {
}
TargettedEvent::~TargettedEvent() {}

/* Null Types
-------------------------------------------------- */

// Null Event Emitter
void NullEventEmitter::emit(std::queue<Event::Ptr>& events) {}

// Null Event Handler
void NullEventHandler::handle(const Event::Ptr e) {}
