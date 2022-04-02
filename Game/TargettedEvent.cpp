#include "TargettedEvent.h"

// Targetted Event
const EventType TargettedEvent::TYPE;
TargettedEvent::TargettedEvent(Event::Ptr event, const EventHandler::WPtr target)
	: Event(TYPE), event(event), target(target) {
}
TargettedEvent::~TargettedEvent() {}
