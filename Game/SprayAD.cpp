#include "SprayAD.h"

SprayAD::SprayAD(
    float maxAttackAngle,
    bool canRotate,
    float firingAngleRotationSpeed
) :
    maxAttackAngle(maxAttackAngle),
    canRotate(canRotate),
    firingAngleRotationSpeed(firingAngleRotationSpeed)
{}

SprayAD::Ptr SprayAD::create(
    float maxAttackAngle,
    bool canRotate,
    float firingAngleRotationSpeed
) {
    return Ptr(new SprayAD(
        maxAttackAngle,
        canRotate,
        firingAngleRotationSpeed
    ));
}
