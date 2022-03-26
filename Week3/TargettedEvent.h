#pragma once

#include "Event.h"
#include "EventHandler.h"

// Targetted Event
class TargettedEvent : public Event {
public:
	using Ptr = std::shared_ptr<TargettedEvent>;
	using UPtr = std::unique_ptr<TargettedEvent>;
	using WPtr = std::weak_ptr<TargettedEvent>;
	static const EventType TYPE;

	Event::Ptr event;
	const EventHandler::WPtr target;

	TargettedEvent(Event::Ptr event, const EventHandler::WPtr target);
	virtual ~TargettedEvent();
};
