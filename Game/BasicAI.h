#pragma once

#include <memory>
#include <list>

#include "EventEmitter.h"

#include "Tracker.h"
#include "AITypes.h"

#include "TargettingStrategy.h"
#include "MovementStrategy.h"

class BasicAI final : public EventEmitter {
private:
    std::list<ControlledObject> controlledObjects;
    std::list<TargetObject> targetCandidates;

    template <class T>
    void dropExiredFrom(std::list<T>& objects);

    const TargettingStrategy::Ptr targettingStrategy;
    const MovementStrategy::Ptr movementStrategy;

    BasicAI(
        const TargettingStrategy::Ptr targettingStrategy,
        const MovementStrategy::Ptr movementStrategy
    );

public:
    using Ptr = std::shared_ptr<BasicAI>;
    using UPtr = std::unique_ptr<BasicAI>;
    using WPtr = std::weak_ptr<BasicAI>;

    static UPtr create(
        const TargettingStrategy::Ptr targettingStrategy,
        const MovementStrategy::Ptr movementStrategy
    );

    void addControlledObject(HasPhysOf<NewtonianPhysModel>::WPtr object);
    void addTarget(HasPhysOf<NewtonianPhysModel>::WPtr object);

    virtual void emit(std::queue<Event::Ptr>& events) override;
};

template <class T>
inline void BasicAI::dropExiredFrom(std::list<T>& objects) {
    objects.remove_if([](const T& object) { return object.obj.expired(); });
}
