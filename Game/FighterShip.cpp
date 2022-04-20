#include "FighterShip.h"

#include <string>
#include "ptrcast.h"

#include "BasicMovement.h"
#include "BulletAttack.h"

FighterShip::FighterShip(FighterShipSpec::Ptr spec) :
    Ship(spec),
    HasEventHandlerOf(MultiEventHandler::create()),
    HasComponent<BasicMovement>(BasicMovement::create()),
    HasComponent<BulletAttack>(BulletAttack::create(spec->bulletImage))
{
    trackPhysObserver(HasComponent<BasicMovement>::component());
    trackPhysObserver(HasComponent<BulletAttack>::component());
    trackEventEmitterObserver(HasComponent<BulletAttack>::component());

    eventHandler().add(HasComponent<BasicMovement>::component());
    eventHandler().add(HasComponent<BulletAttack>::component());
}

const ObjectFactory FighterShip::factory = [](ObjectSpec::UPtr spec) {
    return FighterShip::Ptr(new FighterShip(static_unique_pointer_cast<FighterShipSpec>(move(spec))));
};

void FighterShip::setObjectEventFactory(ObjectEventFactory::Ptr objectEventFactory) {
    // Delegate to components that need it (this class doesn't need it)
    HasComponent<BulletAttack>::component()->setObjectEventFactory(objectEventFactory);
}
