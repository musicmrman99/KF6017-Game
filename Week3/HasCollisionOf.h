#pragma once

#include "Derived.h"
#include "ptrcast.h"

#include "HasCollision.h"
#include "CollisionObserverOf.h"

// GameObject Trait: Adds consideration by some Collision, per the particular CollisionModel type given.
// 
// Warning: Using objects of this type as a HasCollision to set the CollisionModel to something not derived
//          from TCollisionModel, then using these functions to get it as a TCollisionModel is equivalent to
//          performing an ordinary dangerous static_cast, and is therefore liable to cause memory
//          corruption and/or access violations.
template <class TCollisionModel>
class HasCollisionOf : public HasCollision {
private:
    // A (T)ype (D)erived from (CollisionModel)
    using TDCollisionModel = Derived<TCollisionModel, CollisionModel>;
    using TDCollisionModelPtr = std::shared_ptr<TDCollisionModel>;
    using TDCollisionModelWPtr = std::weak_ptr<TDCollisionModel>;

public:
    using Ptr = std::shared_ptr<HasCollisionOf<TDCollisionModel>>;
    using UPtr = std::unique_ptr<HasCollisionOf<TDCollisionModel>>;
    using WPtr = std::weak_ptr<HasCollisionOf<TDCollisionModel>>;

    HasCollisionOf(TDCollisionModelPtr collisionModel) : HasCollision(collisionModel) {}

    // Note: Non-virtual hiding is intended.

    // Make changing the CollisionModel reflected in the given CollisionModel-dependant object.
    void trackCollisionObserver(typename CollisionObserverOf<TDCollisionModel>::WPtr collisionObserver) { HasCollision::trackCollisionObserver(collisionObserver); }

    // Useful for setting the CollisionModel as an observer of another model
    TDCollisionModelWPtr collisionModelWPtr() const { return static_weak_pointer_cast<TDCollisionModel>(HasCollision::collisionModelWPtr()); }
    TDCollisionModel& collisionModel() const { return static_cast<TDCollisionModel&>(HasCollision::collisionModel()); }
    void setCollisionModel(TDCollisionModelPtr collisionModel) {
        if (collisionModel) HasCollision::setCollisionModel(collisionModel);
    }
};
