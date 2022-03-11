#include "KeyMap.h"

KeyMap::KeyMap(GameObject::Ptr object) : object(object) {}

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

void KeyMap::bind(Control::UPtr control, EventEmitter::Ptr action) {
    map.push_back(std::pair<Control::UPtr, EventEmitter::Ptr>(move(control), action));
}

void KeyMap::bind(Control* control, EventEmitter* action) {
    map.push_back(std::pair<Control::UPtr, EventEmitter::Ptr>(control, action));
}
