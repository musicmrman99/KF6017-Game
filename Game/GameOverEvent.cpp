#include "GameOverEvent.h"

const EventType GameOverEvent::TYPE;
GameOverEvent::GameOverEvent() : Event(TYPE) {}
GameOverEvent::Ptr GameOverEvent::create() {
    return Ptr(new GameOverEvent());
}
