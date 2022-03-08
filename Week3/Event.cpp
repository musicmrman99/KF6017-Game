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

Event::Event(const EventType::Ptr& type) : type(type) {}
Event::~Event() {}

/* Event Emitter
-------------------- */

EventEmitter::~EventEmitter() {}

/* Event Handler
-------------------- */

EventHandler::~EventHandler() {}

/* Targetted Event
-------------------- */

TargettedEvent::TargettedEvent(const EventType::Ptr& type, const TargetPtr target)
	: Event(type), target(target) {
}
TargettedEvent::~TargettedEvent() {}

/* Event Type Manager
-------------------------------------------------- */

BaseEventTypeNPtr EventTypeManager::ANY_EVENT_TYPE = BaseEventTypeN::create(new EventCategory());

EventTypeManager::EventTypeManager() {}

const EventCategory::Ptr EventTypeManager::getRootEventType() {
	return std::static_pointer_cast<EventCategory>(ANY_EVENT_TYPE->getValue());
}

// Create new category/type nodes (private)

BaseEventType::Ptr EventTypeManager::registerNew(BaseEventType* newBaseEventType, const BaseEventType::Ptr& parentCategory) {
	std::optional<BaseEventTypeNPtr> maybeParentCategory = BaseEventTypeN::find(ANY_EVENT_TYPE, parentCategory);
	if (!maybeParentCategory) return nullptr; // Should never happen if you use registerBaseEventType() for all categories

	BaseEventTypeNPtr node = BaseEventTypeN::create(newBaseEventType);
	(*maybeParentCategory)->addChild(node);
	return node->getValue();
}
BaseEventType::Ptr EventTypeManager::registerNew(BaseEventType* newBaseEventType) {
	// Re-implement for efficiency (only 3 lines of duplication)
	BaseEventTypeNPtr node = BaseEventTypeN::create(newBaseEventType);
	ANY_EVENT_TYPE->addChild(node);
	return node->getValue();
}

// Create new categories

EventCategory::Ptr EventTypeManager::registerNewCategory(EventCategory* newEventCategory, const EventCategory::Ptr& parentCategory) {
	return std::static_pointer_cast<EventCategory>(registerNew(newEventCategory, parentCategory));
}
EventCategory::Ptr EventTypeManager::registerNewCategory(EventCategory* newEventCategory) {
	return std::static_pointer_cast<EventCategory>(registerNew(newEventCategory));
}
EventCategory::Ptr EventTypeManager::registerNewCategory(const EventCategory::Ptr& parentCategory) {
	return std::static_pointer_cast<EventCategory>(registerNew(new EventCategory(), parentCategory));
}
EventCategory::Ptr EventTypeManager::registerNewCategory() {
	return std::static_pointer_cast<EventCategory>(registerNew(new EventCategory()));
}

// Create new types

EventType::Ptr EventTypeManager::registerNewType(EventType* newEventCategory, const EventCategory::Ptr& parentCategory) {
	return std::static_pointer_cast<EventType>(registerNew(newEventCategory, parentCategory));
}
EventType::Ptr EventTypeManager::registerNewType(EventType* newEventCategory) {
	return std::static_pointer_cast<EventType>(registerNew(newEventCategory));
}
EventType::Ptr EventTypeManager::registerNewType(const EventCategory::Ptr& parentCategory) {
	return std::static_pointer_cast<EventType>(registerNew(new EventType(), parentCategory));
}
EventType::Ptr EventTypeManager::registerNewType() {
	return std::static_pointer_cast<EventType>(registerNew(new EventType()));
}

// Type checker

bool EventTypeManager::isOfType(const BaseEventType::Ptr& type, const BaseEventType::Ptr& against) {
	std::optional<std::vector<BaseEventType::Ptr>> path = BaseEventTypeN::findValuePath(ANY_EVENT_TYPE, type);
	if (!path) return false; // No such type
	return std::find(path->rbegin(), path->rend(), against) != path->rend(); // More likely to find it at the end, so iterate backwards
}

/* Common Implementations
-------------------------------------------------- */

/* Null Event Emitter
-------------------- */

void NullEventEmitter::emit(std::queue<Event::Ptr>& events) {}

/* Basic Event Emitter
-------------------- */

BasicEventEmitter::BasicEventEmitter(const EventType::Ptr& eventType) : eventType(eventType) {}
void BasicEventEmitter::emit(std::queue<Event::Ptr>& events) {
	events.push(Event::Ptr(new Event(eventType)));
}
