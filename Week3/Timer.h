#pragma once

#include "SelfReferencing.h"

#include "Event.h"
#include "EventHandler.h"
#include "ObjectEventCreator.h"

class Timer final : public EventEmitter, public ObjectEventCreator, public SelfReferencing<Timer> {
private:
	double limit;
	double created;
	EventHandler::WPtr listener;

	Timer(double limit, EventHandler::WPtr listener);

public:
	using Ptr = std::shared_ptr<Timer>;

	static Ptr create(double limit, EventHandler::WPtr listener);
	virtual void emit(std::queue<Event::Ptr>& events) override;
};

class TimerEvent final : public Event {
private:
	TimerEvent(const Timer::WPtr timer);

public:
	static const EventType TYPE;

	const Timer::WPtr timer;
	static Ptr create(const Timer::WPtr timer);
};
