#include "HasPhys.h"

HasPhys::HasPhys(PhysModel::Ptr physModel) {
    setPhysModel(physModel);
}
HasPhys::~HasPhys() {}

void HasPhys::trackPhysObserver(std::weak_ptr<PhysObserver> physObserver) {
    if (auto po = physObserver.lock()) {
        po->updatePhysModel(_physModel);
        _observerTracker.track(physObserver);
    }
}

PhysModel& HasPhys::physModel() const { return *_physModel; }

void HasPhys::setPhysModel(PhysModel::Ptr physModel) {
    // Set the model
    if (physModel) _physModel = physModel;
    else _physModel = PhysModel::Ptr(new NullPhysModel()); // Do not allow nullptr

    // Update all observers
    bool areAnyExpired = false;
    for (PhysObserver::WPtr& observer : _observerTracker.getTracked()) {
        if (auto ob = observer.lock()) {
            ob->updatePhysModel(physModel);
        } else {
            areAnyExpired = true;
        }
    }
    if (areAnyExpired) _observerTracker.dropExpired();
}

void HasPhys::beforePhys() {};
void HasPhys::phys() {
    physModel().run();
}
