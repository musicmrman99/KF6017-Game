#include "HasCollision.h"

HasCollision::HasCollision(CollisionModel::Ptr collisionModel) {
    setCollisionModel(collisionModel);
}
HasCollision::~HasCollision() {}

void HasCollision::trackCollisionObserver(CollisionObserver::WPtr collisionObserver) {
    if (auto po = collisionObserver.lock()) {
        po->updateCollisionModel(_collisionModel);
        _observerTracker.track(collisionObserver);
    }
}

CollisionModel::WPtr HasCollision::collisionModelWPtr() const { return _collisionModel; }
CollisionModel& HasCollision::collisionModel() const { return *_collisionModel; }

void HasCollision::setCollisionModel(CollisionModel::Ptr collisionModel) {
    // Set the model
    if (collisionModel) _collisionModel = collisionModel;
    else _collisionModel = CollisionModel::Ptr(new NullCollisionModel()); // Do not allow nullptr

    // Update all observers
    bool areAnyExpired = false;
    for (CollisionObserver::WPtr& observer : _observerTracker.getTracked()) {
        if (auto ob = observer.lock()) {
            ob->updateCollisionModel(collisionModel);
        } else {
            areAnyExpired = true;
        }
    }
    if (areAnyExpired) _observerTracker.dropExpired();
}

void HasCollision::updateCollision() {
    _collisionModel->updateCollision();
}
