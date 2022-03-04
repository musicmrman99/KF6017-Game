#pragma once

#include <queue>

#include "Derived.h"
#include "Symbol.h"
#include "Node.h"

/* Core Concepts
-------------------------------------------------- */

// EventType
class BaseEventType : public Symbol {
public:
	BaseEventType();
	virtual ~BaseEventType();
};

using EventType = Node<BaseEventType>;
using EventTypePtr = Node<BaseEventType>::ValuePtr;
using EventTypeNodePtr = Node<BaseEventType>::NodePtr;

// Event
class Event {
public:
	const EventTypePtr& type;
	Event(const EventTypePtr& type);
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
	static EventTypeNodePtr eventTypeRoot;

	EventTypeManager();

public:
	static const EventTypeNodePtr& getRootEventType();

	static EventTypeNodePtr registerNewType(BaseEventType* newEventType, const EventType::ValuePtr& parentEventType);
	static EventTypeNodePtr registerNewType(BaseEventType* newEventType);
	static EventTypeNodePtr registerNewType(const EventType::ValuePtr& parentEventType);
	static EventTypeNodePtr registerNewType();

	static bool isOfType(const EventTypePtr& type, const EventTypePtr& against);
};

/* Common Implementations
-------------------------------------------------- */

class NullEventEmitter : public EventEmitter {
public:
	virtual void emit(std::queue<Event>& events);
};

class BasicEventEmitter : public EventEmitter {
private:
	const EventTypePtr eventType;

public:
	BasicEventEmitter(const EventTypePtr& eventType);
	virtual void emit(std::queue<Event>& events);
};
