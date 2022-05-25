#include "SprayAS.h"

#include "SprayAttack.h"
#include "TargettedEvent.h"

#include "NearestUntilDestroyedTD.h"
#include "SprayAD.h"

SprayAS::Ptr SprayAS::create() {
    return Ptr(new SprayAS());
}

void SprayAS::attack(
    std::list<ControlledObject>& controlledObjects,
    std::queue<Event::Ptr>& events
) {
    for (ControlledObject& controlledObject : controlledObjects) {
        NearestUntilDestroyedTD::Ptr targettingData = std::static_pointer_cast<NearestUntilDestroyedTD>(controlledObject.targettingData);
        SprayAD::Ptr attackData = std::static_pointer_cast<SprayAD>(controlledObject.attackData);

        HasPhysOf<NewtonianPhysModel>::Ptr controlledObjLock = controlledObject.obj.lock();
        HasPhysOf<NewtonianPhysModel>::Ptr targetObjLock = targettingData->objTarget.obj.lock();

        Vector2D toTarget = targetObjLock->physModel().pos() - controlledObjLock->physModel().pos();

        Vector2D diff = controlledObjLock->physModel().rot().rotatedBy(-controlledObjLock->physModel().rotVel() - toTarget.angle());
        float angleDiff = diff.angle();

        /* Rotate Firing Angle (if possible)
        -------------------- */

        if (
            attackData->canRotate &&
            angleDiff >= attackData->maxAttackAngle &&
            angleDiff <= 2 * M_PI - attackData->maxAttackAngle
        ) {
            // Request to rotate firing instrument
            events.push(TargettedEvent::Ptr(new TargettedEvent(
                SprayAttack::RotateFiringAngleEvent::Ptr(new SprayAttack::RotateFiringAngleEvent(attackData->firingAngleRotationSpeed)),
                std::dynamic_pointer_cast<EventHandler>(controlledObjLock)
            )));
        }

        /* Attack (if appropriate)
        -------------------- */

        // This is a 'vaguely aiming in the right direction' kind of AI - ie. not very clever.
        // It's a spray attack, though, so some shots are somewhat likely to hit.
        Vector2D tmp;
        tmp.setBearing(attackData->maxAttackAngle, 1.0f);
        MyDrawEngine::GetInstance()->DrawLine(
            targetObjLock->physModel().pos() + Vector2D(100, 200),
            targetObjLock->physModel().pos() + Vector2D(100, 200) + tmp * 100,
            MyDrawEngine::BLUE
        );
        tmp.setBearing(-attackData->maxAttackAngle, 1.0f);
        MyDrawEngine::GetInstance()->DrawLine(
            targetObjLock->physModel().pos() + Vector2D(100, 200),
            targetObjLock->physModel().pos() + Vector2D(100, 200) + tmp * 100,
            MyDrawEngine::BLUE
        );
        MyDrawEngine::GetInstance()->DrawLine(
            targetObjLock->physModel().pos() + Vector2D(100, 200),
            targetObjLock->physModel().pos() + Vector2D(100, 200) + diff * 100,
            MyDrawEngine::RED
        );
        if (
            angleDiff < attackData->maxAttackAngle ||
            angleDiff > 2 * M_PI - attackData->maxAttackAngle
        ) {
            // Request to fire
            events.push(TargettedEvent::Ptr(new TargettedEvent(
                SprayAttack::FireEvent::Ptr(new SprayAttack::FireEvent()),
                std::dynamic_pointer_cast<EventHandler>(controlledObjLock)
            )));
        }
    }
}
