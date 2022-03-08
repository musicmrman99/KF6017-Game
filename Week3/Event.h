#pragma once

#include <queue>

#include "Derived.h"
#include "Symbol.h"
#include "Node.h"

/* Core Concepts
-------------------------------------------------- */

class BaseEventType : public Symbol {
public:
	using Ptr = std::shared_ptr<BaseEventType>;

	BaseEventType();
	virtual ~BaseEventType();
};

// A category of event types akin to an 'abstract' event type. Cannot be used to create an event
class EventCategory : public BaseEventType {
public:
	using Ptr = std::shared_ptr<EventCategory>;

	EventCategory();
	virtual ~EventCategory();
};

// An event type.
class EventType : public BaseEventType {
public:
	using Ptr = std::shared_ptr<EventType>;

	EventType();
	virtual ~EventType();
};

using BaseEventTypeN = Node<BaseEventType>;
using BaseEventTypeNPtr = BaseEventTypeN::NodePtr;

using EventCategoryN = Node<EventCategory>;
using EventCategoryNPtr = EventCategoryN::NodePtr;

using EventTypeN = Node<EventType>;
using EventTypeNPtr = EventTypeN::NodePtr;

// Event
class Event {
public:
	using Ptr = std::shared_ptr<Event>;

	const EventType::Ptr& type;
	Event(const EventType::Ptr& type);
	virtual ~Event();
};

// Event Emitter
class EventEmitter : public Symbol {
public:
	virtual void emit(std::queue<Event::Ptr>& events) = 0;
};

// Event Handler
class EventHandler : public Symbol {
public:
	virtual void handle(const Event::Ptr e) = 0;
};

class TargettedEvent : public Event {
public:
	using Ptr = std::shared_ptr<TargettedEvent>;
	using TargetPtr = std::weak_ptr<EventHandler>;

	const TargetPtr target;
	TargettedEvent(const EventType::Ptr& type, const TargetPtr target);
};

/* Event Type Manager
-------------------------------------------------- */

// Hacky global state (would be better if it were compile-time checked)
class EventTypeManager final {
private:
	static BaseEventTypeNPtr ANY_EVENT_TYPE; // The root event type category

	EventTypeManager();

	static BaseEventType::Ptr registerNew(BaseEventType* newBaseEventType, const BaseEventType::Ptr& parentCategory);
	static BaseEventType::Ptr registerNew(BaseEventType* newBaseEventType);

public:
	static const EventCategory::Ptr getRootEventType();

	static EventCategory::Ptr registerNewCategory(EventCategory* newEventCategory, const EventCategory::Ptr& parentCategory);
	static EventCategory::Ptr registerNewCategory(EventCategory* newEventCategory);
	static EventCategory::Ptr registerNewCategory(const EventCategory::Ptr& parentCategory);
	static EventCategory::Ptr registerNewCategory();

	static EventType::Ptr registerNewType(EventType* newEventType, const EventCategory::Ptr& category);
	static EventType::Ptr registerNewType(EventType* newEventType);
	static EventType::Ptr registerNewType(const EventCategory::Ptr& category);
	static EventType::Ptr registerNewType();

	static bool isOfType(const BaseEventType::Ptr& type, const BaseEventType::Ptr& against);
};

/* Common Implementations
-------------------------------------------------- */

class NullEventEmitter : public EventEmitter {
public:
	virtual void emit(std::queue<Event::Ptr>& events) override;
};

class BasicEventEmitter : public EventEmitter {
private:
	const EventType::Ptr& eventType;

public:
	BasicEventEmitter(const EventType::Ptr& eventType);
	virtual void emit(std::queue<Event::Ptr>& events) override;
};
