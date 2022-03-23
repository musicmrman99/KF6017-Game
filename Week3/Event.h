#pragma once

#include <queue>
#include <memory>

#include "Derived.h"
#include "Symbol.h"
#include "Node.h"

/* Core Types
-------------------------------------------------- */

class EventType : public Symbol {};

// Event
class Event {
public:
	using Ptr = std::shared_ptr<Event>;
	using UPtr = std::unique_ptr<Event>;
	using WPtr = std::weak_ptr<Event>;

	const EventType& type;

	Event(const EventType& type);
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

/* Null Types
-------------------------------------------------- */

// Null Event Emitter
class NullEventEmitter final : public EventEmitter {
public:
	using Ptr = std::shared_ptr<NullEventEmitter>;
	using UPtr = std::unique_ptr<NullEventEmitter>;
	using WPtr = std::weak_ptr<NullEventEmitter>;

	virtual void emit(std::queue<Event::Ptr>& events) override;
};

// Null Event Handler
class NullEventHandler final : public EventHandler {
public:
	using Ptr = std::shared_ptr<NullEventHandler>;
	using UPtr = std::unique_ptr<NullEventHandler>;
	using WPtr = std::weak_ptr<NullEventHandler>;

	virtual void handle(const Event::Ptr e) override;
};
