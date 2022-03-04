#include "Event.h"

/* Core Concepts
-------------------------------------------------- */

/* Event Category (a set of event types)
-------------------- */

EventCategory::EventCategory() : Symbol() {}
EventCategory::~EventCategory() {}

/* Event Type (an event type)
-------------------- */

EventType::EventType() : EventCategory() {}
EventType::~EventType() {}

/* Event
-------------------- */

Event::Event(const EventTypeVPtr& type) : type(type) {};

/* Event Type Manager
-------------------------------------------------- */

EventCategoryNPtr EventTypeManager::eventTypeRoot = EventCategoryN::create(new EventCategory());

EventTypeManager::EventTypeManager() {}

const EventCategoryVPtr EventTypeManager::getRootEventType() {
	return eventTypeRoot->getValue();
}

// Create new categories

EventCategoryVPtr EventTypeManager::registerNewCategory(EventCategory* newEventCategory, const EventCategoryVPtr& parentCategory) {
	std::optional<EventCategoryNPtr> maybeParentCategory = EventCategoryN::find(eventTypeRoot, parentCategory);
	if (!maybeParentCategory) return nullptr; // Should never happen if you use registerEventCategory() for all categories

	EventCategoryNPtr node = EventCategoryN::create(newEventCategory);
	(*maybeParentCategory)->addChild(node);
	return node->getValue();
}
EventCategoryVPtr EventTypeManager::registerNewCategory(EventCategory* newEventCategory) {
	// Re-implement for efficiency (only 3 lines of duplication)
	EventCategoryNPtr node = EventCategoryN::create(newEventCategory);
	eventTypeRoot->addChild(node);
	return node->getValue();
}
EventCategoryVPtr EventTypeManager::registerNewCategory(const EventCategoryVPtr& parentCategory) {
	return registerNewCategory(new EventCategory(), parentCategory);
}
EventCategoryVPtr EventTypeManager::registerNewCategory() {
	return registerNewCategory(new EventCategory());
}

// Create new types
// These are not semantically equivalent to the above - and cannot re-use their implementations

EventTypeVPtr EventTypeManager::registerNewType(EventType* newEventCategory, const EventCategoryVPtr& parentCategory) {
	return std::static_pointer_cast<EventType>(registerNewCategory(newEventCategory, parentCategory));
}
EventTypeVPtr EventTypeManager::registerNewType(EventType* newEventCategory) {
	return std::static_pointer_cast<EventType>(registerNewCategory(newEventCategory));
}
EventTypeVPtr EventTypeManager::registerNewType(const EventCategoryVPtr& parentCategory) {
	return std::static_pointer_cast<EventType>(registerNewCategory(new EventType(), parentCategory));
}
EventTypeVPtr EventTypeManager::registerNewType() {
	return std::static_pointer_cast<EventType>(registerNewCategory(new EventType()));
}

// Type checker

bool EventTypeManager::isOfType(const EventCategoryVPtr& type, const EventCategoryVPtr& against) {
	std::optional<std::vector<EventCategoryVPtr>> path = EventCategoryN::findValuePath(eventTypeRoot, type);
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

BasicEventEmitter::BasicEventEmitter(const EventTypeVPtr& eventType) : eventType(eventType) {}
void BasicEventEmitter::emit(std::queue<Event>& events) {
	events.push(Event(eventType));
}
