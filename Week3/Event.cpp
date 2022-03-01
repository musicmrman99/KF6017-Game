#include "Event.h"

/* Core Concepts
-------------------------------------------------- */

/* Event Type (group)
-------------------- */

BaseEventType::BaseEventType() : Symbol() {}
BaseEventType::~BaseEventType() {}

/* Event
-------------------- */

Event::Event(const EventTypePtr& type) : type(type) {};

/* Event Type Manager
-------------------------------------------------- */

EventTypeNodePtr EventTypeManager::eventTypeRoot = EventType::create(new BaseEventType());

EventTypeManager::EventTypeManager() {}

const EventTypeNodePtr& EventTypeManager::getRootEventType() {
	return eventTypeRoot;
}

EventTypeNodePtr EventTypeManager::create(const EventType::ValuePtr& parentEventType) {
	EventTypeNodePtr node = EventType::create(new BaseEventType());

	std::optional<EventTypeNodePtr> maybeParent = EventType::find(eventTypeRoot, parentEventType);
	if (maybeParent) {
		(*maybeParent)->addChild(node);
	}

	return node;
}

EventTypeNodePtr EventTypeManager::create() {
	EventTypeNodePtr node = EventType::create(new BaseEventType());
	eventTypeRoot->addChild(node);
	return node;
}

bool EventTypeManager::isOfType(const EventTypePtr& type, const EventTypePtr& against) {
	std::optional<std::vector<EventTypePtr>> path = EventType::findValuePath(eventTypeRoot, type);
	if (!path) return false; // No such type
	return std::find(path->rbegin(), path->rend(), against) != path->rend(); // More likely to find it at the end, so iterate backwards
}

/* Common Implementations
-------------------------------------------------- */

/* Null Event Emitter
-------------------- */

void NullEventEmitter::emit(std::queue<Event>& events) {}

/* Basic Event Emitter
-------------------- */

BasicEventEmitter::BasicEventEmitter(const EventTypePtr& eventType) : eventType(eventType) {}
void BasicEventEmitter::emit(std::queue<Event>& events) {
	events.push(Event(eventType));
}
