#pragma once

#include <vector>

template <class Action>
class ActionSource {
public:
    virtual std::vector<Action> getActions() = 0;
};