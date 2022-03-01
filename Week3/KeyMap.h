#pragma once

#include <vector>

#include "Event.h"
#include "Controls.h"

/*
 * Maps Controls to the given type of Events and collates which actions should be
 * run on each frame on whichever actor(s) this KeyMap is set as the EventEmitter for.
 */
class KeyMap : public EventEmitter {
private:
    // Controls are held/iterated in insertion order,
    // which may impact the order of actions.
    std::vector<std::pair<Control*, EventEmitter*>> map;

public:
    KeyMap() {}
    ~KeyMap() {
        for (std::pair<Control*, EventEmitter*> mapping : map) {
            if (mapping.first) {
                delete mapping.first;
            }
        }
    }

    // Implement EventEmitter
    virtual void emit(std::queue<Event>& events) override {
        for (std::pair<Control*, EventEmitter*> binding : map) {
            if (binding.first->isActive()) {
                binding.second->emit(events);
            }
        }
    }

    // Bind
    void bind(Control* control, EventEmitter* action) {
        map.push_back(std::pair<Control*, EventEmitter*>(control, action));
    }
};
