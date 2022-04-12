#pragma once

#include <queue>
#include <memory>

#include "Symbol.h"

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
