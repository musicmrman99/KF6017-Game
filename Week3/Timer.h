#pragma once

#include "Event.h"

class Timer final : public EventEmitter {
private:
	double limit;
	double created;

public:
	Timer(double limit);
	virtual void emit(std::queue<Event::Ptr>& events) override;
};

class TimerEvent final : public Event {
private:
	const Timer& timer;

public:
	TimerEvent(const Timer& timer);
};
