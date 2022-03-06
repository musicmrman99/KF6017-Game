#include "Event.h"

/* Core Concepts
-------------------------------------------------- */

BaseEventType::BaseEventType() : Symbol() {}
BaseEventType::~BaseEventType() {}

/* Event Category (a set of event types)
-------------------- */

EventCategory::EventCategory() : BaseEventType() {}
EventCategory::~EventCategory() {}

/* Event Type (an event type)
-------------------- */

EventType::EventType() : BaseEventType() {}
EventType::~EventType() {}

/* Event
-------------------- */

Event::Event(const EventTypeVPtr& type) : type(type) {}
Event::~Event() {}

/* Targetted Event
-------------------- */

TargettedEvent::TargettedEvent(const EventTypeVPtr& type, const TargetPtr target)
	: Event(type), target(target) {
}

/* Event Type Manager
-------------------------------------------------- */

BaseEventTypeNPtr EventTypeManager::ANY_EVENT_TYPE = BaseEventTypeN::create(new EventCategory());

EventTypeManager::EventTypeManager() {}

const EventCategoryVPtr EventTypeManager::getRootEventType() {
	return std::static_pointer_cast<EventCategory>(ANY_EVENT_TYPE->getValue());
}

// Create new category/type nodes (private)

BaseEventTypeVPtr EventTypeManager::registerNew(BaseEventType* newBaseEventType, const BaseEventTypeVPtr& parentCategory) {
	std::optional<BaseEventTypeNPtr> maybeParentCategory = BaseEventTypeN::find(ANY_EVENT_TYPE, parentCategory);
	if (!maybeParentCategory) return nullptr; // Should never happen if you use registerBaseEventType() for all categories

	BaseEventTypeNPtr node = BaseEventTypeN::create(newBaseEventType);
	(*maybeParentCategory)->addChild(node);
	return node->getValue();
}
BaseEventTypeVPtr EventTypeManager::registerNew(BaseEventType* newBaseEventType) {
	// Re-implement for efficiency (only 3 lines of duplication)
	BaseEventTypeNPtr node = BaseEventTypeN::create(newBaseEventType);
	ANY_EVENT_TYPE->addChild(node);
	return node->getValue();
}

// Create new categories

EventCategoryVPtr EventTypeManager::registerNewCategory(EventCategory* newEventCategory, const EventCategoryVPtr& parentCategory) {
	return std::static_pointer_cast<EventCategory>(registerNew(newEventCategory, parentCategory));
}
EventCategoryVPtr EventTypeManager::registerNewCategory(EventCategory* newEventCategory) {
	return std::static_pointer_cast<EventCategory>(registerNew(newEventCategory));
}
EventCategoryVPtr EventTypeManager::registerNewCategory(const EventCategoryVPtr& parentCategory) {
	return std::static_pointer_cast<EventCategory>(registerNew(new EventCategory(), parentCategory));
}
EventCategoryVPtr EventTypeManager::registerNewCategory() {
	return std::static_pointer_cast<EventCategory>(registerNew(new EventCategory()));
}

// Create new types

EventTypeVPtr EventTypeManager::registerNewType(EventType* newEventCategory, const EventCategoryVPtr& parentCategory) {
	return std::static_pointer_cast<EventType>(registerNew(newEventCategory, parentCategory));
}
EventTypeVPtr EventTypeManager::registerNewType(EventType* newEventCategory) {
	return std::static_pointer_cast<EventType>(registerNew(newEventCategory));
}
EventTypeVPtr EventTypeManager::registerNewType(const EventCategoryVPtr& parentCategory) {
	return std::static_pointer_cast<EventType>(registerNew(new EventType(), parentCategory));
}
EventTypeVPtr EventTypeManager::registerNewType() {
	return std::static_pointer_cast<EventType>(registerNew(new EventType()));
}

// Type checker

bool EventTypeManager::isOfType(const BaseEventTypeVPtr& type, const BaseEventTypeVPtr& against) {
	std::optional<std::vector<BaseEventTypeVPtr>> path = BaseEventTypeN::findValuePath(ANY_EVENT_TYPE, type);
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
