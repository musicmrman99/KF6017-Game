#pragma once

#include "Derived.h"
#include "ptrcast.h"

#include "HasPhys.h"
#include "PhysObserverOf.h"

// GameObject Trait: Adds the Physics component slot and restricts it to holding a particular
// Physics Model type.
// 
// Warning: Using objects of this type as a HasPhys to set the PhysModel to something not derived
//          from TPhysModel, then using these functions to get it as a TPhysModel is equivalent to
//          performing an ordinary dangerous static_cast, and is therefore liable to cause memory
//          corruption and/or access violations.
template <class TPhysModel>
class HasPhysOf : public HasPhys {
private:
    // A (T)ype (D)erived from (PhysModel)
    using TDPhysModel = Derived<TPhysModel, PhysModel>;
    using TDPhysModelPtr = std::shared_ptr<TDPhysModel>;
    using TDPhysModelWPtr = std::weak_ptr<TDPhysModel>;

public:
    HasPhysOf(TDPhysModelPtr physModel) : HasPhys(physModel) {}

    // Note: Non-virtual hiding is intended.

    // Make changing the TDPhysModel reflected in the given TDPhysModel-dependant object.
    void trackPhysObserver(typename PhysObserverOf<TDPhysModel>::WPtr physObserver) { HasPhys::trackPhysObserver(physObserver); }

    // Useful for setting the PhysModel as an observer of another model
    TDPhysModelWPtr physModelWPtr() const { return static_weak_pointer_cast<TDPhysModel>(HasPhys::physModelWPtr()); }
    TDPhysModel& physModel() const { return static_cast<TDPhysModel&>(HasPhys::physModel()); }
    void setPhysModel(TDPhysModelPtr physModel) {
        if (physModel) HasPhys::setPhysModel(physModel);
    }
};
