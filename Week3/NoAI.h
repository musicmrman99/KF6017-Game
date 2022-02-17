#pragma once

#include "ActionSource.h"

template <class Action>
class NoAI : public ActionSource<Action> {
    // Return no actions.
    virtual std::vector<Action*> getActions() const {
        static std::vector<Action*> noActions;
        return noActions;
    }
};
