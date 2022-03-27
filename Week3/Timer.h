#pragma once

#include "Referencing.h"

#include "Event.h"
#include "EventHandler.h"
#include "Controller.h"
#include "ObjectEventCreator.h"

// An EventEmitter intended to be used with Controller that emits an event after
// a given duration.
class Timer final :
	public EventEmitter,
	public ObjectEventCreator,
	public Referencing<Controller>,
	public Referencing<Timer>
{
private:
	double timeLimit;
	double timeCreated;
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
