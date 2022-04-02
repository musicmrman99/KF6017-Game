#pragma once

#include <memory>

#include "Tracker.h"

#include "CollisionModel.h"
#include "CollisionObserver.h"

/*
GameObject Trait: Adds consideration by the Collision controllers.

This includes:
- The CollisionModel (use collisionModel()/setCollisionModel() for get/set)
- An observer tracker for the CollisionModel dependants (use trackCollisionObserver() to add a dependant object)
- The Collision Tracking pseudo-'lifecycle method' (updateCollision())
*/
class HasCollision {
private:
    Tracker<CollisionObserver> _observerTracker;
    CollisionModel::Ptr _collisionModel;

public:
    using Ptr = std::shared_ptr<HasCollision>;
    using UPtr = std::unique_ptr<HasCollision>;
    using WPtr = std::weak_ptr<HasCollision>;

    // Initialise the Collision component slot
    HasCollision(CollisionModel::Ptr collisionModel);

    virtual ~HasCollision();

    // Make changing the CollisionModel reflected in the given CollisionModel-dependant object.
    void trackCollisionObserver(CollisionObserver::WPtr collisionObserver);

    CollisionModel::WPtr collisionModelWPtr() const;            // Useful for setting the CollisionModel as an observer of another model
    CollisionModel& collisionModel() const;                     // Get the CollisionModel
    void setCollisionModel(CollisionModel::Ptr collisionModel); // Set the CollisionModel (and update all observers)

    // Run the "updateCollision" pseudo-lifecycle method
    virtual void updateCollision();
};
