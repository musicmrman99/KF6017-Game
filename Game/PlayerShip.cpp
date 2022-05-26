#include "PlayerShip.h"

#include <string>

#include "ptrcast.h"

#include "BulletSpec.h"

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
    // Upgrade tree observers? What actually creates the upgrade events?

    eventHandler().add(HasComponent<BasicMovement>::component());
    eventHandler().add(HasComponent<SprayAttack>::component());
    eventHandler().add(HasComponent<Integrity>::component());
    eventHandler().add(upgradeTreePtr());

    // Thread dependencies
    trackUpgradeTreeObserver(uiModelWPtr());

    // Virtual method initialisation
    initUpgradeTree();
}

const ObjectFactory PlayerShip::factory = [](ObjectSpec::UPtr spec) {
    return GameObject::Ptr(new PlayerShip(static_unique_pointer_cast<PlayerShipSpec>(move(spec))));
};

void PlayerShip::setObjectEventFactory(ObjectEventFactory::Ptr objectEventFactory) {
    // Delegate to components that need it (this class doesn't need it)
    HasComponent<SprayAttack>::component()->setObjectEventFactory(objectEventFactory);
}

// Organise the available upgrades into a tree.
void PlayerShip::buildUpgradeTree(UpgradeTree& upgradeTree) {
    // Formatted the same as tree itself for ease of reading
    const auto& heavyShells = upgradeTree.addUpgrade(PlayerShipUpgrade::HEAVY_SHELLS);
    upgradeTree.addUpgrade(heavyShells, PlayerShipUpgrade::PLASMA_CANNON);
    const auto& armouredHull = upgradeTree.addUpgrade(PlayerShipUpgrade::ARMOURED_HULL);
    upgradeTree.addUpgrade(armouredHull, PlayerShipUpgrade::SHIELDING);
}
