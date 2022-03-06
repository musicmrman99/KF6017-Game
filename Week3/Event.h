#pragma once

#include <queue>

#include "Derived.h"
#include "Symbol.h"
#include "Node.h"

/* Core Concepts
-------------------------------------------------- */

class BaseEventType : public Symbol {
public:
	BaseEventType();
	virtual ~BaseEventType();
};

// A category of event types akin to an 'abstract' event type. Cannot be used to create an event
class EventCategory : public BaseEventType {
public:
	EventCategory();
	virtual ~EventCategory();
};

// An event type.
class EventType : public BaseEventType {
public:
	EventType();
	virtual ~EventType();
};

using BaseEventTypeN = Node<BaseEventType>;
using BaseEventTypeNPtr = BaseEventTypeN::NodePtr;
using BaseEventTypeVPtr = BaseEventTypeN::ValuePtr;

using EventCategoryN = Node<EventCategory>;
using EventCategoryNPtr = EventCategoryN::NodePtr;
using EventCategoryVPtr = EventCategoryN::ValuePtr;

using EventTypeN = Node<EventType>;
using EventTypeNPtr = EventTypeN::NodePtr;
using EventTypeVPtr = EventTypeN::ValuePtr;

// Event
class Event {
public:
	const EventTypeVPtr& type;
	Event(const EventTypeVPtr& type);
	virtual ~Event();
};

// Event Emitter
class EventEmitter : public Symbol {
public:
	virtual void emit(std::queue<Event>& events) = 0;
};

// Event Handler
class EventHandler : public Symbol {
public:
	virtual void handle(const Event& e) = 0;
};

class TargettedEvent : Event {
public:
	using TargetPtr = std::weak_ptr<EventHandler>;

	const TargetPtr target;
	TargettedEvent(const EventTypeVPtr& type, const TargetPtr target);
};

/* Event Type Manager
-------------------------------------------------- */

// Hacky global state (would be better if it were compile-time checked)
class EventTypeManager final {
private:
	static BaseEventTypeNPtr ANY_EVENT_TYPE; // The root event type category

	EventTypeManager();

	static BaseEventTypeVPtr registerNew(BaseEventType* newBaseEventType, const BaseEventTypeVPtr& parentCategory);
	static BaseEventTypeVPtr registerNew(BaseEventType* newBaseEventType);

public:
	static const EventCategoryVPtr getRootEventType();

	static EventCategoryVPtr registerNewCategory(EventCategory* newEventCategory, const EventCategoryVPtr& parentCategory);
	static EventCategoryVPtr registerNewCategory(EventCategory* newEventCategory);
	static EventCategoryVPtr registerNewCategory(const EventCategoryVPtr& parentCategory);
	static EventCategoryVPtr registerNewCategory();

	static EventTypeVPtr registerNewType(EventType* newEventType, const EventCategoryVPtr& category);
	static EventTypeVPtr registerNewType(EventType* newEventType);
	static EventTypeVPtr registerNewType(const EventCategoryVPtr& category);
	static EventTypeVPtr registerNewType();

	static bool isOfType(const BaseEventTypeVPtr& type, const BaseEventTypeVPtr& against);
};

/* Common Implementations
-------------------------------------------------- */

class NullEventEmitter : public EventEmitter {
public:
	virtual void emit(std::queue<Event>& events);
};

class BasicEventEmitter : public EventEmitter {
private:
	const EventTypeVPtr eventType;

public:
	BasicEventEmitter(const EventTypeVPtr& eventType);
	virtual void emit(std::queue<Event>& events);
};
