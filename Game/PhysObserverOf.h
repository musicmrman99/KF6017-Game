#pragma once

#include "Derived.h"
#include "ptrcast.h"

#include "PhysObserver.h"

// Trait of objects that depend on reading a PhysModel.
template <class TPhysModel>
class PhysObserverOf : public PhysObserver {
private:
    // A (T)ype (D)erived from (PhysModel)
    using TDPhysModel = Derived<TPhysModel, PhysModel>;
    using TDPhysModelWPtr = std::weak_ptr<TDPhysModel>;
    using TDPhysModelPtr = std::shared_ptr<TDPhysModel>;

public:
    using WPtr = std::weak_ptr<PhysObserverOf<TDPhysModel>>;

    const TDPhysModelWPtr physModelWPtr() const {
        return static_weak_pointer_cast<TDPhysModel>(PhysObserver::physModelWPtr());
    }
    TDPhysModel& physModel() const { return *physModelWPtr().lock(); }
};
