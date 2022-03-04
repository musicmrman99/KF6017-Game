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
    // which will impact the order of emitted events.
    std::vector<std::pair<Control*, EventEmitter*>> map;

public:
    KeyMap() {}
    ~KeyMap();

    virtual void emit(std::queue<Event>& events) override;
    void bind(Control* control, EventEmitter* action);
};
