#include "Timer.h"

#include "Game.h"
#include "Event.h"
#include "TargettedEvent.h"
#include "ObjectEvent.h"

// Timer
Timer::Timer(double limit, EventHandler::WPtr listener)
	: created(Game::gt.time()), limit(limit), listener(listener) {
}

Timer::Ptr Timer::create(double limit, EventHandler::WPtr listener) {
	Timer::Ptr timer = Timer::Ptr(new Timer(limit, listener));
	timer->setSelf(timer);
	return timer;
}

void Timer::emit(std::queue<Event::Ptr>& events) {
	if (Game::gt.timeSince(created) > limit) {
		events.push(
			TargettedEvent::UPtr(new TargettedEvent(
				TimerEvent::create(self()),                          // Emit the timer event (with this object as the source)
				listener                                             // To the listener
			))
		);
		events.push(objectEventFactory()->removeController(self())); // Then remove/delete yourself (the timer)
	}
}

// Timer Event
const EventType TimerEvent::TYPE;
TimerEvent::TimerEvent(const Timer::WPtr timer) : Event(TYPE), timer(timer) {}
TimerEvent::Ptr TimerEvent::create(const Timer::WPtr timer) {
	return Ptr(new TimerEvent(timer));
}
