#pragma once

#include <memory>
#include <list>

#include "EventEmitter.h"

#include "ControlledObject.h"

#include "TargettingStrategy.h"
#include "MovementStrategy.h"

class BasicAI final : public EventEmitter {
private:
    std::list<ControlledObject> controlledObjects;

    TargettingStrategy::Ptr _targettingStrategy;
    MovementStrategy::Ptr _movementStrategy;

    BasicAI(
        const TargettingStrategy::Ptr targettingStrategy,
        const MovementStrategy::Ptr movementStrategy
    );

public:
    using Ptr = std::shared_ptr<BasicAI>;
    using UPtr = std::unique_ptr<BasicAI>;
    using WPtr = std::weak_ptr<BasicAI>;

    static UPtr create(
        TargettingStrategy::Ptr targettingStrategy,
        MovementStrategy::Ptr movementStrategy
    );

    void add(
        HasPhysOf<NewtonianPhysModel>::WPtr object,
        TargettingData::Ptr targettingData,
        MovementData::Ptr movementData
    );

    TargettingStrategy::Ptr targettingStrategy() const;
    MovementStrategy::Ptr movementStrategy() const;

    virtual void emit(std::queue<Event::Ptr>& events) override;
};
