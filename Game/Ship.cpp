#include "Ship.h"

#include <string>

#include "ptrcast.h"

#include "BulletSpec.h"

Ship::Ship(ShipSpec::Ptr spec) :
    HasEventEmitterOf(BufferedEventEmitter::UPtr(new BufferedEventEmitter())),
    HasCollisionOf(BasicCollisionModel::create(
        new AngledRectangle2D(spec->collisionSize.XValue, spec->collisionSize.YValue),
        ShipSpec::SHIP_COLLISION,
        {
            ShipSpec::SHIP_COLLISION,
            BulletSpec::BULLET_COLLISION
        }
    )),
    HasPhysOf(NewtonianPhysModel::UPtr(new NewtonianPhysModel(spec->pos, Vector2D(0, 0), spec->rot, 0.0f))),
    HasGraphicsOf(ImageGraphicsModel::UPtr(new ImageGraphicsModel(spec->image)))
{
    // Thread dependencies
    trackPhysObserver(graphicsModelWPtr());
    trackPhysObserver(collisionModelWPtr());
}

void Ship::beforeFrame() {
    physModel().setAccel(Vector2D(0.0f, 0.0f));
    physModel().setRotAccel(0.0f);
}
