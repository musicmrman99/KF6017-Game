#pragma once

#include <functional>
#include <memory>

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
    std::shared_ptr<ActionSource<Action>> _actionSource;
    std::shared_ptr<Derived<TPhysModel, PhysModel>> _physModel;

public:
    // You must give an actual actionSource and TPhysModel
    GameObject(std::shared_ptr<ActionSource<Action>> actionSource, std::shared_ptr<TPhysModel> physModel) {
        setActionSource(actionSource);
        setPhysModel(physModel);
    }

    virtual ~GameObject() {}

    ActionSource<Action>& actionSource() { return *_actionSource; }
    TPhysModel& physModel() { return *_physModel; }

    void setActionSource(std::shared_ptr<ActionSource<Action>> actionSource) {
        if (actionSource) _actionSource = actionSource;
    }
    void setPhysModel(std::shared_ptr<TPhysModel> physModel) {
        if (physModel) _physModel = physModel;
    }
};
