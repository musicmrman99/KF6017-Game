#include "NearestUntilDestroyedTS.h"

#include "ptrcast.h"

#include "NearestUntilDestroyedTD.h"

NearestUntilDestroyedTS::Ptr NearestUntilDestroyedTS::create() {
    return Ptr(new NearestUntilDestroyedTS());
}

void NearestUntilDestroyedTS::selectTargets(std::list<ControlledObject>& controlledObjects) {
    TargettingStrategy::selectTargets(controlledObjects);

    Vector2D toTarget;
    const TargetObject* nearestTarget; // Non-const pointer to const object
    float nearestDist;

    for (ControlledObject& controlledObject : controlledObjects) {
        NearestUntilDestroyedTD::Ptr targettingData = std::static_pointer_cast<NearestUntilDestroyedTD>(controlledObject.targettingData);

        // Note: I would rather use min_element(start_iter, end_iter, comparator),
        //       but the following does a rolling calculation, so doesn't re-calculate
        //       the so-far-nearest target's vector & magnitude for each element, so
        //       is faster.
        if (targettingData->objTarget.obj.expired() && targetCandidates().size() > 0) {
            // Select the first target initially
            nearestTarget = &targetCandidates().front();
            toTarget.set(
                nearestTarget->obj.lock()->physModel().pos() -
                controlledObject.obj.lock()->physModel().pos()
            );
            nearestDist = toTarget.magnitude();

            // Find closest target (if any other target candidates are closer than the first)
            float checkDist;
            for (auto target = std::next(targetCandidates().begin()); target != targetCandidates().end(); target++) {
                toTarget = target->obj.lock()->physModel().pos() - controlledObject.obj.lock()->physModel().pos();
                checkDist = toTarget.magnitude();
                if (checkDist < nearestDist) {
                    nearestTarget = &*target; // `&*` to convert iterator to pointer
                    nearestDist = checkDist;
                }
            }

            // Select the found target
            targettingData->objTarget = *nearestTarget; // Will this copy? Shouldn't matter even if it does.
        }
    }
}
