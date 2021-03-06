#include "KeyMap.h"

#include "TargettedEvent.h"

KeyMap::KeyMap(HasEventHandler::Ptr object) : object(object) {}
KeyMap::UPtr KeyMap::create(HasEventHandler::Ptr object) { return UPtr(new KeyMap(object)); }

void KeyMap::bind(Control::UPtr control, EventEmitter::Ptr action) {
    map.push_back(std::pair<Control::UPtr, EventEmitter::Ptr>(move(control), action));
}

void KeyMap::bind(Control* control, EventEmitter* action) {
    map.push_back(std::pair<Control::UPtr, EventEmitter::Ptr>(control, action));
}

void KeyMap::emit(std::queue<Event::Ptr>& events) {
    static std::queue<Event::Ptr> eventsBuffer;
    for (std::pair<Control::UPtr, EventEmitter::Ptr>& binding : map) {
        if (binding.first->isActive()) {
            binding.second->emit(eventsBuffer);
        }
    }
    while (!eventsBuffer.empty()) {
        const Event::Ptr& e = eventsBuffer.front();
        events.push(TargettedEvent::Ptr(new TargettedEvent(e, object)));
        eventsBuffer.pop();
    }
}
