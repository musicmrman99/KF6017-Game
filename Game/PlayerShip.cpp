#include "PlayerShip.h"

#include <string>

#include "ptrcast.h"

#include "BulletSpec.h"

/* Upgrades
-------------------------------------------------- */

const Upgrade PlayerShipUpgrade::SHIP(L"Ship"); // Root Upgrade

const Upgrade PlayerShipUpgrade::LOAD_OPTIMISATION(L"Load Optimisation");
const Upgrade PlayerShipUpgrade::SPACIAL_COMPRESSION(L"Spacial Compression");
const Upgrade PlayerShipUpgrade::COOPERATION(L"Cooperation");
const Upgrade PlayerShipUpgrade::OPTIMAL_SELECTION(L"Optimal Selection");

const Upgrade PlayerShipUpgrade::FRONT_THRUSTERS(L"Front Thrusters");
const Upgrade PlayerShipUpgrade::REAR_THRUSTERS(L"Rear Thrusters");
const Upgrade PlayerShipUpgrade::OVERDRIVE(L"Overdrive");
const Upgrade PlayerShipUpgrade::HYPER_JUMP(L"Hyper Jump");

const Upgrade PlayerShipUpgrade::HEAVY_SHELLS(L"Heavy Shells");
const Upgrade PlayerShipUpgrade::FRONT_AUTO_CANNONS(L"Front Auto-Cannons");
const Upgrade PlayerShipUpgrade::REAR_AUTO_CANNONS(L"Rear Auto-Cannons");
const Upgrade PlayerShipUpgrade::IONIC_SHELLS(L"Ionic Shells");

const Upgrade PlayerShipUpgrade::WORKER_DRONE(L"Worker Drone");
const Upgrade PlayerShipUpgrade::ARMOURED_DRONE(L"Armoured Drone");
const Upgrade PlayerShipUpgrade::MINE(L"Mine");
const Upgrade PlayerShipUpgrade::FIGHTER_DRONE(L"Fighter Drone");

/* Player Ship
-------------------------------------------------- */

PlayerShip::PlayerShip(PlayerShipSpec::Ptr spec) :
    Ship(spec),
    HasEventHandlerOf(MultiEventHandler::create()),
    HasComponent<BasicMovement>(BasicMovement::create()),
    HasComponent<BulletAttack>(BulletAttack::create(spec->bulletImage)),
    HasUpgradeTree(PlayerShipUpgrade::SHIP),
    HasUIOf(UpgradeTreeUI::create())
{
    // Thread dependencies
    trackPhysObserver(HasComponent<BasicMovement>::component());
    trackPhysObserver(HasComponent<BulletAttack>::component());
    trackEventEmitterObserver(HasComponent<BulletAttack>::component());
    // Upgrade tree observers? What actually creates the upgrade events?

    eventHandler().add(HasComponent<BasicMovement>::component());
    eventHandler().add(HasComponent<BulletAttack>::component());
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
    HasComponent<BulletAttack>::component()->setObjectEventFactory(objectEventFactory);
}

// Organise the available upgrades into a tree.
void PlayerShip::buildUpgradeTree(UpgradeTree& upgradeTree) {
    // Formatted the same as tree itself for ease of reading
    const auto& loadOptimisation = upgradeTree.addUpgrade(PlayerShipUpgrade::LOAD_OPTIMISATION);
    upgradeTree.addUpgrade(loadOptimisation, PlayerShipUpgrade::SPACIAL_COMPRESSION);
    const auto& cooperation = upgradeTree.addUpgrade(loadOptimisation, PlayerShipUpgrade::COOPERATION);
    upgradeTree.addUpgrade(cooperation, PlayerShipUpgrade::OPTIMAL_SELECTION);

    const auto& rearThrusters = upgradeTree.addUpgrade(PlayerShipUpgrade::REAR_THRUSTERS);
    upgradeTree.addUpgrade(rearThrusters, PlayerShipUpgrade::FRONT_THRUSTERS);
    const auto& overdrive = upgradeTree.addUpgrade(rearThrusters, PlayerShipUpgrade::OVERDRIVE);
    upgradeTree.addUpgrade(overdrive, PlayerShipUpgrade::HYPER_JUMP);

    const auto& heavyShells = upgradeTree.addUpgrade(PlayerShipUpgrade::HEAVY_SHELLS);
    upgradeTree.addUpgrade(heavyShells, PlayerShipUpgrade::IONIC_SHELLS);
    const auto& frontAutoCannons = upgradeTree.addUpgrade(heavyShells, PlayerShipUpgrade::FRONT_AUTO_CANNONS);
    upgradeTree.addUpgrade(frontAutoCannons, PlayerShipUpgrade::REAR_AUTO_CANNONS);

    const auto& workerDrone = upgradeTree.addUpgrade(PlayerShipUpgrade::WORKER_DRONE);
    upgradeTree.addUpgrade(workerDrone, PlayerShipUpgrade::ARMOURED_DRONE);
    const auto& mine = upgradeTree.addUpgrade(workerDrone, PlayerShipUpgrade::MINE);
    upgradeTree.addUpgrade(mine, PlayerShipUpgrade::FIGHTER_DRONE);
}
