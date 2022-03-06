#include "KeyMap.h"

KeyMap::~KeyMap() {
    for (std::pair<Control*, EventEmitter*> mapping : map) {
        if (mapping.first) delete mapping.first;
        if (mapping.second) delete mapping.second;
    }
}

void KeyMap::emit(std::queue<Event::Ptr>& events) {
    for (std::pair<Control*, EventEmitter*> binding : map) {
        if (binding.first->isActive()) {
            binding.second->emit(events);
        }
    }
}

void KeyMap::bind(Control* control, EventEmitter* action) {
    map.push_back(std::pair<Control*, EventEmitter*>(control, action));
}
