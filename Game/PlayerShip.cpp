#include "PlayerShip.h"

#include <string>
#include "ptrcast.h"

#include "BulletSpec.h"
#include "GameOverEvent.h"

/* Upgrades
-------------------------------------------------- */

const Upgrade PlayerShipUpgrade::SHIP(L"Ship"); // Root Upgrade

const Upgrade PlayerShipUpgrade::HEAVY_SHELLS(L"Heavy Shells");
const Upgrade PlayerShipUpgrade::PLASMA_CANNON(L"Plasma Cannon");
const Upgrade PlayerShipUpgrade::ARMOURED_HULL(L"Armoured Hull");
const Upgrade PlayerShipUpgrade::SHIELDING(L"Shielding");

/* Player Ship
-------------------------------------------------- */

PlayerShip::PlayerShip(PlayerShipSpec::Ptr spec) :
    Ship(spec),
    HasEventHandlerOf(MultiEventHandler::create()),
    HasComponent<BasicMovement>(BasicMovement::create(0.1f, 0.008f)),
    HasComponent<SprayAttack>(SprayAttack::create(spec->bulletImage, spec->bulletDamage)),
    HasComponent<Integrity>(Integrity::create(spec->bulletImage, spec->maxIntegrity)), // FIXME: Bullet image
    HasUpgradeTree(PlayerShipUpgrade::SHIP),
    HasUIOf(UpgradeTreeUI::create())
{
    // Thread dependencies
    trackPhysObserver(HasComponent<BasicMovement>::component());
    trackPhysObserver(HasComponent<SprayAttack>::component());
    trackEventEmitterObserver(HasComponent<SprayAttack>::component());
    trackEventEmitterObserver(HasComponent<Integrity>::component());
    // Upgrade tree observers? What actually creates the upgrade events?

    eventHandler().add(HasComponent<BasicMovement>::component());
    eventHandler().add(HasComponent<SprayAttack>::component());
    eventHandler().add(HasComponent<Integrity>::component());
    eventHandler().add(upgradeTreePtr());

    trackUpgradeTreeObserver(uiModelWPtr());

    // Virtual method initialisation
    initUpgradeTree();
}

const ObjectFactory PlayerShip::factory = [](ObjectSpec::UPtr spec) {
    GameObject::Ptr ship = GameObject::Ptr(new PlayerShip(static_unique_pointer_cast<PlayerShipSpec>(move(spec))));
    std::dynamic_pointer_cast<HasComponent<Integrity>>(ship)->component()->setRef(ship);
    return ship;
};

void PlayerShip::setObjectEventFactory(ObjectEventFactory::Ptr objectEventFactory) {
    // Delegate to components that need it (this class doesn't need it)
    HasComponent<SprayAttack>::component()->setObjectEventFactory(objectEventFactory);
    HasComponent<Integrity>::component()->setObjectEventFactory(objectEventFactory);
}

// Organise the available upgrades into a tree.
void PlayerShip::buildUpgradeTree(UpgradeTree& upgradeTree) {
    // Formatted the same as tree itself for ease of reading
    const auto& heavyShells = upgradeTree.addUpgrade(PlayerShipUpgrade::HEAVY_SHELLS);
    upgradeTree.addUpgrade(heavyShells, PlayerShipUpgrade::PLASMA_CANNON);
    const auto& armouredHull = upgradeTree.addUpgrade(PlayerShipUpgrade::ARMOURED_HULL);
    upgradeTree.addUpgrade(armouredHull, PlayerShipUpgrade::SHIELDING);
}

void PlayerShip::afterFrame() {
    // This should be in a UI graphics model, but ... I'm out of time.
    float percent = HasComponent<Integrity>::component()->integrity() / HasComponent<Integrity>::component()->maxIntegrity();

    int lineLength = 100;
    MyDrawEngine::GetInstance()->DrawLine(
        physModel().pos() + Vector2D(50, lineLength / 2),
        physModel().pos() + Vector2D(50, lineLength / 2),
        MyDrawEngine::GREY
    );
    MyDrawEngine::GetInstance()->DrawLine(
        physModel().pos() + Vector2D(50, lineLength / 2),
        physModel().pos() + Vector2D(50, (lineLength / 2) * (1 - percent)),
        MyDrawEngine::BLUE
    );
    MyDrawEngine::GetInstance()->WriteDouble(
        physModel().pos() + Vector2D(50, 0),
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

void PlayerShip::beforeDestroy() {
    eventEmitter().enqueue(GameOverEvent::create());
}
