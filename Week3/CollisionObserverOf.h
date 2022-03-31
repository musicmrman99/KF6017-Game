#pragma once

#include "Derived.h"
#include "ptrcast.h"

#include "CollisionObserver.h"

// Trait of objects that depend on reading an CollisionModel.
template <class TCollisionModel>
class CollisionObserverOf : public CollisionObserver {
private:
    // A (T)ype (D)erived from (CollisionModel)
    using TDCollisionModel = Derived<TCollisionModel, CollisionModel>;
    using TDCollisionModelPtr = std::shared_ptr<TDCollisionModel>;
    using TDCollisionModelWPtr = std::weak_ptr<TDCollisionModel>;

public:
    using WPtr = std::weak_ptr<CollisionObserverOf<TDCollisionModel>>;

    const TDCollisionModelWPtr collisionModelWPtr() const {
        return static_weak_pointer_cast<TDCollisionModel>(CollisionObserver::collisionModelWPtr());
    }
    TDCollisionModel& collisionModel() const { return *collisionModelWPtr().lock(); }
};
