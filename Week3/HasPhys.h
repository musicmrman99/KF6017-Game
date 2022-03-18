#pragma once

#include <memory>

#include "Tracker.h"

#include "PhysModel.h"
#include "PhysObserver.h"

/*
GameObject Trait : Adds the Physics component slot.

This includes:
- The PhysModel (use physModel()/setPhysModel() for get/set)
- An observer tracker for the PhysModel dependants (use trackPhysObserver() to add a dependant object)
- The physics lifecycle methods (beforePhys() and phys())
*/
class HasPhys {
private:
    Tracker<PhysObserver> _observerTracker;
    PhysModel::Ptr _physModel;

public:
    using Ptr = std::shared_ptr<HasPhys>;
    using UPtr = std::unique_ptr<HasPhys>;
    using WPtr = std::weak_ptr<HasPhys>;

    // Initialise the Physics component slot
    HasPhys(PhysModel::Ptr physModel);

    virtual ~HasPhys();

    // Make changing the PhysModel reflected in the given PhysModel-dependant object.
    void trackPhysObserver(std::weak_ptr<PhysObserver> physObserver);

    PhysModel& physModel() const;                // Get the PhysModel
    void setPhysModel(PhysModel::Ptr physModel); // Set the PhysModel (and update all observers)

    virtual void beforePhys(); // Run the "beforePhys" lifecycle method
    virtual void phys();       // Run the "phys" lifecycle method
};
