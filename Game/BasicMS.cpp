#include "BasicMS.h"

#include <cmath>
#include "realmod.h"

#include "HasComponent.h"
#include "TargettedEvent.h"

#include "ControlledObject.h"
#include "NearestUntilDestroyedTD.h"
#include "BasicMD.h"

#include "mydrawengine.h"

BasicMS::Ptr BasicMS::create() {
    return Ptr(new BasicMS());
}

void BasicMS::moveObjects(
    std::list<ControlledObject>& controlledObjects,
    std::queue<Event::Ptr>& events
) {
    for (ControlledObject& controlledObject : controlledObjects) {
        NearestUntilDestroyedTD::Ptr targettingData = std::static_pointer_cast<NearestUntilDestroyedTD>(controlledObject.targettingData);
        BasicMD::Ptr movementData = std::static_pointer_cast<BasicMD>(controlledObject.movementData);

        HasPhysOf<NewtonianPhysModel>::Ptr controlledObjLock = controlledObject.obj.lock();
        HasPhysOf<NewtonianPhysModel>::Ptr targetObjLock = targettingData->objTarget.obj.lock();

        BasicMovement::Ptr controlledObjMovement = std::dynamic_pointer_cast<HasComponent<BasicMovement>>(controlledObjLock)->component();
        if (!controlledObjMovement) continue; // If it can't move, don't try to move

        Vector2D toTarget = targetObjLock->physModel().pos() - controlledObjLock->physModel().pos();

        /* Movement Targetting
        -------------------- */

        // Thank you Chris!

        // Rotate (and modulo it to keep it in bounds)
        movementData->targetRot += movementData->targetRotVel;
        if (movementData->targetRot > 2.0f * (float) M_PI) {
            movementData->targetRot = realmod(movementData->targetRot, 2.0f * (float) M_PI);
        }

        // Calculate new target position
        Vector2D toDistance;
        toDistance.setBearing(movementData->targetRot, movementData->targetDistance);

        // y = A sin(B(x + C)) + D (from https://www.mathsisfun.com/algebra/amplitude-period-frequency-phase-shift.html)
        // We don't need C or D, so y = A sin(Bx) = amp * sin(freq * rot)
        Vector2D toOffset;
        toOffset.setBearing(
            movementData->targetRot,
            movementData->targetOffsetAmplitude * sinf(movementData->targetOffsetFrequency * movementData->targetRot)
        );

        Vector2D targetPos = targetObjLock->physModel().pos() + toDistance + toOffset;
        Vector2D toTargetPos = targetPos - controlledObjLock->physModel().pos();

        MyDrawEngine::GetInstance()->FillCircle(targetPos, 5, MyDrawEngine::RED);

        /* Rotation & Movement
        -------------------- */

        Vector2D targetVelocity = toTargetPos.unitVector() * movementData->maximumSpeed - controlledObjLock->physModel().vel();

        // See Appendix 1 for the mathematical derivation of the following (this + Rotation section).

        // Given current rotational velocity, if you start decelerating now, where would you be
        // by the time you finished?
        const Vector2D& curRot = controlledObjLock->physModel().rot();
        Vector2D newRot = curRot.rotatedBy( // Adjust for modular angles
            controlledObjLock->physModel().rotVel() * abs(controlledObjLock->physModel().rotVel())
            / (2.0f * controlledObjMovement->rotationalThrust())
        );

        // Da = (Ea - Ta) % 2 PI
        // The difference between expected final vector angle and the target vector angle
        Vector2D expectedDiff = newRot.rotatedBy(-targetVelocity.angle());
        float expectedAngleDiff = expectedDiff.angle();

        MyDrawEngine::GetInstance()->DrawLine(
            targetObjLock->physModel().pos() + Vector2D(100, 200),
            targetObjLock->physModel().pos() + Vector2D(100, 200) + expectedDiff * 100,
            MyDrawEngine::RED);

        /* Rotation
        -------------------- */

        // Which way should we turn based on the final destination?
        if (expectedAngleDiff < M_PI) {
            events.push(TargettedEvent::Ptr(new TargettedEvent(
                BasicMovement::TurnLeftThrustEvent::Ptr(new BasicMovement::TurnLeftThrustEvent()),
                std::dynamic_pointer_cast<EventHandler>(controlledObjLock)
            )));

        // Technically, this should be <, not <=, but considering the target is likely
        // to be constantly moving, it won't make much difference.
        } else {
            events.push(TargettedEvent::Ptr(new TargettedEvent(
                BasicMovement::TurnRightThrustEvent::Ptr(new BasicMovement::TurnRightThrustEvent()),
                std::dynamic_pointer_cast<EventHandler>(controlledObjLock)
            )));
        }

        /* Movement
        -------------------- */

        if (
            expectedAngleDiff < movementData->accelMaxAngle ||
            expectedAngleDiff > 2 * M_PI - movementData->accelMaxAngle
        ) {
            // Request to move
            events.push(TargettedEvent::Ptr(new TargettedEvent(
                BasicMovement::MainThrustEvent::Ptr(new BasicMovement::MainThrustEvent()),
                std::dynamic_pointer_cast<EventHandler>(controlledObjLock)
            )));
        }
    }
}

/*
Appendix 1
--------------------

  From: https://courses.lumenlearning.com/physics/chapter/2-5-motion-equations-for-constant-acceleration-in-one-dimension/
x = x[0] + ((v[0] + v[n]) / 2) * t

  v[n] = 0 is the target:
x = x[0] + v[0]/2 * t

  Aside: from the vel/accel equasion (for constant `a`), solve for t:
a = (v[n] - v[0]) / t
t = (v[n] - v[0]) / a
  v[n] = 0 is the target
t = -v[0] / a

  Substitute using known values:
x = x[0] + v[0]/2 * (-v[0] / a)
  Simplify:
x = x[0] - v[0]/2 * v[0]/a
x = x[0] - v[0]^2 / 2a

  For some reason in practice this doesn't work, as the sign of v[0] matters, so:
x = x[0] - v[0]*abs(v[0]) / 2a

  Remember to adjust for modular angles in the range [0, 2pi).
*/
