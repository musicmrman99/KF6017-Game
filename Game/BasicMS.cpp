#include "BasicMS.h"

#include <cmath>

#include "HasComponent.h"
#include "TargettedEvent.h"

#include "ControlledObject.h"
#include "NearestUntilDestroyedTD.h"

#include "mydrawengine.h"

BasicMS::BasicMS(float targetDistance) : targetDistance(targetDistance) {}

BasicMS::Ptr BasicMS::create(float targetDistance) {
    return Ptr(new BasicMS(targetDistance));
}

void BasicMS::moveObjects(
	std::list<ControlledObject>& controlledObjects,
	std::queue<Event::Ptr>& events
) {
	for (ControlledObject& controlledObject : controlledObjects) {
        NearestUntilDestroyedTD::Ptr targettingData = std::static_pointer_cast<NearestUntilDestroyedTD>(controlledObject.targettingData);

        HasPhysOf<NewtonianPhysModel>::Ptr controlledObjLock = controlledObject.obj.lock();
        HasPhysOf<NewtonianPhysModel>::Ptr targetObjLock = targettingData->objTarget.obj.lock();

        BasicMovement::Ptr controlledObjMovement = std::dynamic_pointer_cast<HasComponent<BasicMovement>>(controlledObjLock)->component();
        if (!controlledObjMovement) continue; // If it can't move, don't try to move

        Vector2D toTarget = targetObjLock->physModel().pos() - controlledObjLock->physModel().pos();

        /* Movement Targetting
        -------------------- */

        

        /* Rotation
        -------------------- */

        /*
        // See Appendix 1 for the mathematical derivation of the following.

        // Given current rotational velocity, if you start decelerating now, where would you be
        // by the time you finished?
        const Vector2D& curRot = controlledObjLock->physModel().rot();
        Vector2D newRot = curRot.rotatedBy( // Adjust for modular angles
            controlledObjLock->physModel().rotVel() * abs(controlledObjLock->physModel().rotVel())
            / (2.0f * controlledObjMovement->rotationalThrust())
        );

        // Which way should we turn based on the final destination?
        if (newRot.rotatedBy(-toTarget.angle()).angle() < M_PI) {
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
        */

        // Dodge colliding with the enemy
        /*if (toTarget.magnitude() < 2.0f) {
            events.push(TargettedEvent::Ptr(new TargettedEvent(
                BasicMovement::TurnRightThrustEvent::Ptr(new BasicMovement::TurnRightThrustEvent()),
                std::dynamic_pointer_cast<EventHandler>(controlledObject.obj.lock())
            )));
        }*/

        /* Movement
        -------------------- */

        /*
        Vector2D avgRot = (toTarget.unitVector() + controlledObjLock->physModel().rot().unitVector()) / 2;
        Vector2D avgVel = (toTarget.unitVector() + controlledObjLock->physModel().vel().unitVector()) / 2;
        MyDrawEngine::GetInstance()->DrawLine(
            targetObjLock->physModel().pos()+Vector2D(100, 300),
            targetObjLock->physModel().pos()+Vector2D(100, 300) + avgRot * 100,
            MyDrawEngine::RED);
        MyDrawEngine::GetInstance()->DrawLine(
            targetObjLock->physModel().pos()+Vector2D(100, 200),
            targetObjLock->physModel().pos()+Vector2D(100, 200) + avgVel * 100,
            MyDrawEngine::RED);

        // How close it must be facing the target before it will accellerate.
        // The lower, the more 'slidey'.
        static constexpr float focus = 0.8f;
        
        // How eager it is to turn, regardless of the direction it is facing.
        // The lower, the more 'spiky' its movement will be, the higher, the more 'slidy'.
        static constexpr float eagerness = 0.5f;

        // How fast it's willing to move.
        static constexpr float topSpeed = 3.0f;

        // If you're facing the right direction, and moving in the wrong direction or too slowly, then accellerate
        if (avgRot.magnitude() > focus && (avgVel.magnitude() <= eagerness || controlledObjLock->physModel().vel().magnitude() < topSpeed)) {
            // Try to move
            events.push(TargettedEvent::Ptr(new TargettedEvent(
                BasicMovement::MainThrustEvent::Ptr(new BasicMovement::MainThrustEvent()),
                std::dynamic_pointer_cast<EventHandler>(controlledObjLock)
            )));
        }
        */
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
