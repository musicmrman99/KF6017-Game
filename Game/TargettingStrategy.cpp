#include "TargettingStrategy.h"

#include "DropExpiredFrom.h"

TargettingStrategy::~TargettingStrategy() {}

void TargettingStrategy::addTarget(HasPhysOf<NewtonianPhysModel>::WPtr object) {
    _targetCandidates.push_back(TargetObject { object });
}

const std::list<TargetObject>& TargettingStrategy::targetCandidates() {
    return _targetCandidates;
}

void TargettingStrategy::selectTargets(std::list<ControlledObject>& controlledObjects) {
    dropExiredFrom(_targetCandidates);
}
