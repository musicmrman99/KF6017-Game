#include "KeyMap.h"

void KeyMap::emit(std::queue<Event::Ptr>& events) {
    for (std::pair<Control::UPtr, EventEmitter::Ptr>& binding : map) {
        if (binding.first->isActive()) {
            binding.second->emit(events);
        }
    }
}

void KeyMap::bind(Control::UPtr control, EventEmitter::Ptr action) {
    map.push_back(std::pair<Control::UPtr, EventEmitter::Ptr>(move(control), action));
}

void KeyMap::bind(Control* control, EventEmitter* action) {
    map.push_back(std::pair<Control::UPtr, EventEmitter::Ptr>(control, action));
}
