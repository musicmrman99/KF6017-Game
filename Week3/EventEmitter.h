#pragma once

#include "Event.h"

// Event Emitter
class EventEmitter : public Symbol {
public:
	using Ptr = std::shared_ptr<EventEmitter>;
	using UPtr = std::unique_ptr<EventEmitter>;
	using WPtr = std::weak_ptr<EventEmitter>;

	virtual void emit(std::queue<Event::Ptr>& events) = 0;
	virtual ~EventEmitter();
};

// Null Event Emitter
class NullEventEmitter final : public EventEmitter {
public:
	using Ptr = std::shared_ptr<NullEventEmitter>;
	using UPtr = std::unique_ptr<NullEventEmitter>;
	using WPtr = std::weak_ptr<NullEventEmitter>;

	virtual void emit(std::queue<Event::Ptr>& events) override;
};
