#include "FighterShip.h"

#include <string>
#include "ptrcast.h"

#include "BasicMovement.h"
#include "SprayAttack.h"

FighterShip::FighterShip(FighterShipSpec::Ptr spec) :
    Ship(spec),
    HasEventHandlerOf(MultiEventHandler::create()),
    HasComponent<BasicMovement>(BasicMovement::create(0.1f, 0.008f)),
    HasComponent<SprayAttack>(SprayAttack::create(spec->bulletImage, spec->bulletDamage)),
    HasComponent<Integrity>(Integrity::create(spec->bulletImage, spec->maxIntegrity)) // FIXME: Bullet image
{
    trackPhysObserver(HasComponent<BasicMovement>::component());
    trackPhysObserver(HasComponent<SprayAttack>::component());
    trackEventEmitterObserver(HasComponent<SprayAttack>::component());
    trackEventEmitterObserver(HasComponent<Integrity>::component());

    eventHandler().add(HasComponent<BasicMovement>::component());
    eventHandler().add(HasComponent<SprayAttack>::component());
    eventHandler().add(HasComponent<Integrity>::component());
}

const ObjectFactory FighterShip::factory = [](ObjectSpec::UPtr spec) {
    GameObject::Ptr ship = GameObject::Ptr(new FighterShip(static_unique_pointer_cast<FighterShipSpec>(move(spec))));
    std::dynamic_pointer_cast<HasComponent<Integrity>>(ship)->component()->setRef(ship);
    return ship;
};

void FighterShip::setObjectEventFactory(ObjectEventFactory::Ptr objectEventFactory) {
    // Delegate to components that need it (this class doesn't need it)
    HasComponent<SprayAttack>::component()->setObjectEventFactory(objectEventFactory);
    HasComponent<Integrity>::component()->setObjectEventFactory(objectEventFactory);
}

void FighterShip::afterFrame() {
    // This should be in a UI graphics model, but ... I'm out of time.
    float percent = HasComponent<Integrity>::component()->integrity() / HasComponent<Integrity>::component()->maxIntegrity();

    int lineLength = 100;
    MyDrawEngine::GetInstance()->DrawLine(
        physModel().pos() + Vector2D(100, lineLength / 2),
        physModel().pos() + Vector2D(100, lineLength / 2),
        MyDrawEngine::GREY
    );
    MyDrawEngine::GetInstance()->DrawLine(
        physModel().pos() + Vector2D(100, lineLength / 2),
        physModel().pos() + Vector2D(100, (lineLength / 2) * (1 - percent)),
        MyDrawEngine::BLUE
    );
    MyDrawEngine::GetInstance()->WriteDouble(
        physModel().pos() + Vector2D(100, 0),
        (int) (percent * 100.0f),
        MyDrawEngine::GREY
    );

    // This should be in Integrity + spec, but ... I'm out of time.
    float diff = HasComponent<Integrity>::component()->maxIntegrity() - HasComponent<Integrity>::component()->integrity();

    float repairRate = 0.1f;
    if (diff >= repairRate) {
        HasComponent<Integrity>::component()->shiftIntegrity(repairRate);
    } else if (diff > 0) {
        HasComponent<Integrity>::component()->shiftIntegrity(diff);
    }
}
