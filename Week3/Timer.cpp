#include "Timer.h"

#include "Game.h"

// Timer
Timer::Timer(double limit) : created(Game::gt.time()), limit(limit) {}

void Timer::emit(std::queue<Event::Ptr>& events) {
	if (Game::gt.timeSince(created) > limit) {
		events.push(TimerEvent::Ptr(new TimerEvent(*this)));
	}
}

// Timer Event
TimerEvent::TimerEvent(const Timer& timer) : timer(timer) {}
