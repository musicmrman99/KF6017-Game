#include "Timer.h"

#include "Game.h"
#include "TargettedEvent.h"

/* Timer
-------------------------------------------------- */

Timer::Timer(double limit, EventHandler::WPtr listener)
	: timeCreated(Game::gt.time()), timeLimit(limit), listener(listener) {
}

Timer::Ptr Timer::create(double limit, EventHandler::WPtr listener) {
	Timer::Ptr timer = Timer::Ptr(new Timer(limit, listener));

	// Explicitly disambiguate which Referencing<> base class we want. This is
	// faster than std::static_pointer_cast<>, as it doesn't make a new shared_ptr
	static_cast<Referencing<Timer>*>(timer.get())->setRef(timer);

	return timer;
}

void Timer::emit(std::queue<Event::Ptr>& events) {
	if (Game::gt.timeSince(timeCreated) > timeLimit) {
		// Emit the timer event (with this timer as the source) to the listener
		events.push(
			TargettedEvent::UPtr(new TargettedEvent(
				TimerEvent::create(Referencing<Timer>::ref()),
				listener
			))
		);

		// Then destroy the Controller that holds this Timer
		events.push(objectEventFactory()->destroyObject(Referencing<Controller>::ref()));
	}
}

/* Timer Event
-------------------------------------------------- */

const EventType TimerEvent::TYPE;
TimerEvent::TimerEvent(const Timer::WPtr timer) : Event(TYPE), timer(timer) {}
TimerEvent::Ptr TimerEvent::create(const Timer::WPtr timer) {
	return Ptr(new TimerEvent(timer));
}
