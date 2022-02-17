#pragma once

#include <functional>

#include "Derived.h"

#include "ActionSource.h"
#include "NoAI.h"

#include "PhysModel.h"
#include "NoPhysModel.h"

template <class T, class TPhysModel>
class GameObject {
public:
    // An action that can be applied to a game object.
    using Action = const std::function<void(T& gameObject)>;

private:
    ActionSource<Action>* _actionSource;
    Derived<TPhysModel, PhysModel>* _physModel;

public:
    GameObject(ActionSource<Action>* actionSource, TPhysModel* physModel) {
        setActionSource(actionSource);
        setPhysModel(physModel);
    }

    GameObject() : GameObject(new NoAI<Action>(), new TPhysModel()) {}

    virtual ~GameObject() {
        delete _actionSource;
        delete _physModel;
    }

    virtual ActionSource<Action>& actionSource() { return *_actionSource; }
    virtual TPhysModel& physModel() { return *_physModel; }

    void setActionSource(ActionSource<Action>* actionSource) {
        if (actionSource) _actionSource = actionSource;
    }
    void setPhysModel(TPhysModel* physModel) {
        if (physModel) _physModel = physModel;
    }
};
