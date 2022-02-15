#pragma once

#include <vector>

#include "ActionSource.h"
#include "Controls.h"

/*
 * Maps Controls to the given type of Actionsand collates which actions should be
 * run on each frame on whichever actor(s) this KeyMap is set as the ActionSource for.
 */
template <class Action>
class KeyMap : public ActionSource<Action> {
private:
    // Controls are held/iterated in insertion order,
    // which may impact the order of actions.
    std::vector<std::pair<Control*, Action*>> map;

public:
    KeyMap() {}
    ~KeyMap() {
        for (std::pair<Control*, Action*> mapping : map) {
            if (mapping.first) {
                delete mapping.first;
            }
        }
    }

    // Implement ActionSource<Action>
    virtual std::vector<Action*> getActions() const {
        std::vector<Action*> actions;
        for (std::pair<Control*, Action*> binding : map) {
            if (binding.first->isActive()) {
                actions.push_back(binding.second);
            }
        }
        return actions;
    }

    // Bind
    void bind(Control* control, Action* action) {
        map.push_back(std::pair<Control*, Action*>(control, action));
    }
};
