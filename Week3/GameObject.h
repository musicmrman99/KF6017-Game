#pragma once

#include <functional>

#include "ActionSource.h"
#include "PhysModel.h"

template <class T>
class GameObject {
public:
    // An action that can be applied to a game object.
    using Action = const std::function<void(T& gameObject)>;

private:
    ActionSource<Action>* _actionSource;

public:
    GameObject() : _actionSource(nullptr) {}
    virtual ~GameObject() {}

    virtual ActionSource<Action>& actionSource() { return *_actionSource; }
    void setActionSource(ActionSource<Action>* actionSource) { _actionSource = actionSource; }
};
