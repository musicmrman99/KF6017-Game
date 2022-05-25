#include "FighterShip.h"

#include <string>
#include "ptrcast.h"

#include "BasicMovement.h"
#include "SprayAttack.h"

FighterShip::FighterShip(FighterShipSpec::Ptr spec) :
    Ship(spec),
    HasEventHandlerOf(MultiEventHandler::create()),
    HasComponent<BasicMovement>(BasicMovement::create(0.1f, 0.008f)),
    HasComponent<SprayAttack>(SprayAttack::create(spec->bulletImage))
{
    trackPhysObserver(HasComponent<BasicMovement>::component());
    trackPhysObserver(HasComponent<SprayAttack>::component());
    trackEventEmitterObserver(HasComponent<SprayAttack>::component());

    eventHandler().add(HasComponent<BasicMovement>::component());
    eventHandler().add(HasComponent<SprayAttack>::component());
}

const ObjectFactory FighterShip::factory = [](ObjectSpec::UPtr spec) {
    return FighterShip::Ptr(new FighterShip(static_unique_pointer_cast<FighterShipSpec>(move(spec))));
};

void FighterShip::setObjectEventFactory(ObjectEventFactory::Ptr objectEventFactory) {
    // Delegate to components that need it (this class doesn't need it)
    HasComponent<SprayAttack>::component()->setObjectEventFactory(objectEventFactory);
}
