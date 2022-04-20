#include "BasicMS.h"

#include <cmath>

#include "HasComponent.h"
#include "TargettedEvent.h"

#include "mydrawengine.h"

BasicMS::Ptr BasicMS::create() {
    return Ptr(new BasicMS());
}

void BasicMS::moveObjects(
	std::list<ControlledObject>& controlledObjects,
	std::queue<Event::Ptr>& events
) {
	for (ControlledObject& controlledObject : controlledObjects) {
        HasPhysOf<NewtonianPhysModel>::Ptr controlledObjLock = controlledObject.obj.lock();
        HasPhysOf<NewtonianPhysModel>::Ptr targetObjLock = controlledObject.currentTarget.obj.lock();
        BasicMovement::Ptr controlledObjMovement = std::dynamic_pointer_cast<HasComponent<BasicMovement>>(controlledObjLock)->component();

        Vector2D toTarget = targetObjLock->physModel().pos() - controlledObjLock->physModel().pos();
        const float toTargetAngle = controlledObjLock->physModel().fromRPS(toTarget.angle());

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
                std::dynamic_pointer_cast<EventHandler>(controlledObject.obj.lock())
            )));

        // Technically, this should be <, not <=, but considering the target is likely
        // to be constantly moving, it won't make much difference.
        } else {
            events.push(TargettedEvent::Ptr(new TargettedEvent(
                BasicMovement::TurnRightThrustEvent::Ptr(new BasicMovement::TurnRightThrustEvent()),
                std::dynamic_pointer_cast<EventHandler>(controlledObject.obj.lock())
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
