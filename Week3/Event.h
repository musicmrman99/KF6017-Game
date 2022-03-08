#pragma once

#include <queue>

#include "Derived.h"
#include "Symbol.h"
#include "Node.h"

/* Core Types
-------------------------------------------------- */

// Event
class Event {
public:
	using Ptr = std::shared_ptr<Event>;
	using UPtr = std::unique_ptr<Event>;
	using WPtr = std::weak_ptr<Event>;

	Event();
	Event(const Event::Ptr& ptr);
	virtual ~Event();
};

// Event Emitter
class EventEmitter : public Symbol {
public:
	using Ptr = std::shared_ptr<EventEmitter>;
	using UPtr = std::unique_ptr<EventEmitter>;
	using WPtr = std::weak_ptr<EventEmitter>;

	virtual void emit(std::queue<Event::Ptr>& events) = 0;
	virtual ~EventEmitter();
};

// Event Handler
class EventHandler : public Symbol {
public:
	using Ptr = std::shared_ptr<EventHandler>;
	using UPtr = std::unique_ptr<EventHandler>;
	using WPtr = std::weak_ptr<EventHandler>;

	virtual void handle(const Event::Ptr e) = 0;
	virtual ~EventHandler();
};

// Targetted Event
class TargettedEvent : public Event {
public:
	using Ptr = std::shared_ptr<TargettedEvent>;
	using UPtr = std::unique_ptr<TargettedEvent>;
	using WPtr = std::weak_ptr<TargettedEvent>;

	Event::UPtr event;
	const EventHandler::WPtr target;

	TargettedEvent(Event::UPtr event, const EventHandler::WPtr target);
	virtual ~TargettedEvent();
};

/* Null Types
-------------------------------------------------- */

// Null Event Emitter
class NullEventEmitter final : public EventEmitter {
public:
	virtual void emit(std::queue<Event::Ptr>& events) override;
};

// Null Event Handler
class NullEventHandler final : public EventHandler {
public:
	virtual void handle(const Event::Ptr e) override;
};
