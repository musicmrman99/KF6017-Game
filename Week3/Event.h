#pragma once

#include <queue>

#include "Derived.h"
#include "Symbol.h"
#include "Node.h"

/* Core Concepts
-------------------------------------------------- */

// An Event cannot be created with an abstract event type, but 
class EventCategory : public Symbol {
public:
	EventCategory();
	virtual ~EventCategory();
};

class EventType : public EventCategory {
public:
	EventType();
	virtual ~EventType();
};

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

/* Event Type Manager
-------------------------------------------------- */

// Hacky global state (would be better if it were compile-time checked)
class EventTypeManager final {
private:
	static EventCategoryNPtr eventTypeRoot;

	EventTypeManager();

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

	static bool isOfType(const EventCategoryVPtr& type, const EventCategoryVPtr& against);
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
