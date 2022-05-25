#pragma once

#include <memory>
#include <list>

#include "EventEmitter.h"

#include "ControlledObject.h"

#include "TargettingStrategy.h"
#include "MovementStrategy.h"
#include "AttackStrategy.h"

class BasicAI final : public EventEmitter {
private:
    std::list<ControlledObject> controlledObjects;

    TargettingStrategy::Ptr _targettingStrategy;
    MovementStrategy::Ptr _movementStrategy;
    AttackStrategy::Ptr _attackStrategy;

    BasicAI(
        const TargettingStrategy::Ptr targettingStrategy,
        const MovementStrategy::Ptr movementStrategy,
        const AttackStrategy::Ptr attackStrategy
    );

public:
    using Ptr = std::shared_ptr<BasicAI>;
    using UPtr = std::unique_ptr<BasicAI>;
    using WPtr = std::weak_ptr<BasicAI>;

    static UPtr create(
        TargettingStrategy::Ptr targettingStrategy,
        MovementStrategy::Ptr movementStrategy,
        AttackStrategy::Ptr attackStrategy
    );

    void add(
        HasPhysOf<NewtonianPhysModel>::WPtr object,
        TargettingData::Ptr targettingData,
        MovementData::Ptr movementData,
        AttackData::Ptr attackData
    );

    TargettingStrategy::Ptr targettingStrategy() const;
    MovementStrategy::Ptr movementStrategy() const;
    AttackStrategy::Ptr attackStrategy() const;

    virtual void emit(std::queue<Event::Ptr>& events) override;
};
