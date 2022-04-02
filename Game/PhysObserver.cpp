#include "PhysObserver.h"

PhysModel::WPtr PhysObserver::physModelWPtr() const { return _physModel; }
PhysModel::Ptr PhysObserver::physModel() const { return physModelWPtr().lock(); }
void PhysObserver::updatePhysModel(PhysModel::WPtr physModel) { _physModel = physModel; }
