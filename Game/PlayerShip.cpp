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

/* Event Handler
-------------------------------------------------- */

/* Initialisation
-------------------- */

PlayerShipEventHandler::PlayerShipEventHandler(PlayerShipSpec::Ptr spec)
    : bulletImage(spec->bulletImage),
    engineThrust(0.1f),  // Distance units / second^2
    rotateThrust(0.008f) // Revolutions / second^2
{}

/* Dispatcher
-------------------- */

void PlayerShipEventHandler::handle(const Event::Ptr e) {
    if (e->type == MainThrustEvent::TYPE) mainThrust();
    else if (e->type == TurnLeftThrustEvent::TYPE) turnLeftThrust();
    else if (e->type == TurnRightThrustEvent::TYPE) turnRightThrust();

    else if (e->type == FireEvent::TYPE) fire();

    else if (e->type == UpgradeEvent::TYPE)
        purchaseUpgrade(std::static_pointer_cast<UpgradeEvent>(e)->upgrade);
}

/* Movement
-------------------- */

// Events

const EventType PlayerShipEventHandler::MainThrustEvent::TYPE;
PlayerShipEventHandler::MainThrustEvent::MainThrustEvent() : Event(TYPE) {}
void PlayerShipEventHandler::MainThrustEventEmitter::emit(std::queue<Event::Ptr>& events) {
    events.push(MainThrustEvent::Ptr(new MainThrustEvent()));
}

const EventType PlayerShipEventHandler::TurnLeftThrustEvent::TYPE;
PlayerShipEventHandler::TurnLeftThrustEvent::TurnLeftThrustEvent() : Event(TYPE) {}
void PlayerShipEventHandler::TurnLeftThrustEventEmitter::emit(std::queue<Event::Ptr>& events) {
    events.push(TurnLeftThrustEvent::Ptr(new TurnLeftThrustEvent()));
}

const EventType PlayerShipEventHandler::TurnRightThrustEvent::TYPE;
PlayerShipEventHandler::TurnRightThrustEvent::TurnRightThrustEvent() : Event(TYPE) {}
void PlayerShipEventHandler::TurnRightThrustEventEmitter::emit(std::queue<Event::Ptr>& events) {
    events.push(TurnRightThrustEvent::Ptr(new TurnRightThrustEvent()));
}

// Actions

void PlayerShipEventHandler::mainThrust() {
    physModel().shiftAccel(physModel().rot() * engineThrust);
};

void PlayerShipEventHandler::turnLeftThrust() {
    physModel().shiftRotAccel(-rotateThrust);
};

void PlayerShipEventHandler::turnRightThrust() {
    physModel().shiftRotAccel(rotateThrust);
};

/* Attack
-------------------- */

// Event

const EventType PlayerShipEventHandler::FireEvent::TYPE;
PlayerShipEventHandler::FireEvent::FireEvent() : Event(TYPE) {}
void PlayerShipEventHandler::FireEventEmitter::emit(std::queue<Event::Ptr>& events) {
    events.push(FireEvent::Ptr(new FireEvent()));
}

// Action

void PlayerShipEventHandler::fire() {
    static constexpr float DIST = 40.0f;
    eventEmitter().enqueue(objectEventFactory()->createObject(
        BulletSpec::UPtr(new BulletSpec(
            physModel().pos() + DIST * physModel().rot(),
            physModel().rot(),
            physModel().vel(),
            bulletImage
        ))
    ));
}

void PlayerShipEventHandler::purchaseUpgrade(const Upgrade& upgrade) {
    upgradeTree().purchaseUpgrade(upgrade);
}

/* Player Ship
-------------------------------------------------- */

PlayerShip::PlayerShip(PlayerShipSpec::Ptr spec) :
    Ship(spec),
    HasEventHandlerOf(PlayerShipEventHandler::UPtr(new PlayerShipEventHandler(spec))),
    HasUpgradeTree(PlayerShipUpgrade::SHIP),
    HasUIOf(UpgradeTreeUI::create())
{
    // Thread dependencies
    trackPhysObserver(eventHandlerWPtr());
    trackEventEmitterObserver(eventHandlerWPtr());
    trackUpgradeTreeObserver(eventHandlerWPtr());
    trackUpgradeTreeObserver(uiModelWPtr());

    // Virtual method initialisation
    initUpgradeTree();
}

const ObjectFactory PlayerShip::factory = [](ObjectSpec::UPtr spec) {
    return GameObject::Ptr(new PlayerShip(static_unique_pointer_cast<PlayerShipSpec>(move(spec))));
};

void PlayerShip::setObjectEventFactory(ObjectEventFactory::Ptr objectEventFactory) {
    ObjectEventCreator::setObjectEventFactory(objectEventFactory);
    eventHandler().setObjectEventFactory(objectEventFactory);
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
