#include "Ship.h"

#include <string>

#include "ptrcast.h"

#include "BulletSpec.h"

/* Event Handler
-------------------------------------------------- */

/* Dispatcher
-------------------- */

void ShipEventHandler::handle(const Event::Ptr e) {
    if (e->type == MainThrustEvent::TYPE) mainThrust();
    else if (e->type == TurnLeftThrustEvent::TYPE) turnLeftThrust();
    else if (e->type == TurnRightThrustEvent::TYPE) turnRightThrust();

    else if (e->type == FireEvent::TYPE) fire();

    else if (e->type == UpgradeEvent::TYPE)
        purchaseUpgrade(std::static_pointer_cast<UpgradeEvent>(e)->upgrade);
}

/* Movement
-------------------- */

ShipEventHandler::ShipEventHandler(ShipSpec::Ptr spec)
    : bulletImage(spec->bulletImage),
    engineThrust(0.1f),  // Distance units / second^2
    rotateThrust(0.008f) // Revolutions / second^2
{}

// Events

const EventType ShipEventHandler::MainThrustEvent::TYPE;
ShipEventHandler::MainThrustEvent::MainThrustEvent() : Event(TYPE) {}
void ShipEventHandler::MainThrustEventEmitter::emit(std::queue<Event::Ptr>& events) {
    events.push(MainThrustEvent::Ptr(new MainThrustEvent()));
}

const EventType ShipEventHandler::TurnLeftThrustEvent::TYPE;
ShipEventHandler::TurnLeftThrustEvent::TurnLeftThrustEvent() : Event(TYPE) {}
void ShipEventHandler::TurnLeftThrustEventEmitter::emit(std::queue<Event::Ptr>& events) {
    events.push(TurnLeftThrustEvent::Ptr(new TurnLeftThrustEvent()));
}

const EventType ShipEventHandler::TurnRightThrustEvent::TYPE;
ShipEventHandler::TurnRightThrustEvent::TurnRightThrustEvent() : Event(TYPE) {}
void ShipEventHandler::TurnRightThrustEventEmitter::emit(std::queue<Event::Ptr>& events) {
    events.push(TurnRightThrustEvent::Ptr(new TurnRightThrustEvent()));
}

// Actions

void ShipEventHandler::mainThrust() {
    physModel()->shiftAccel(physModel()->rot() * engineThrust);
};

void ShipEventHandler::turnLeftThrust() {
    physModel()->shiftRotAccel(-rotateThrust);
};

void ShipEventHandler::turnRightThrust() {
    physModel()->shiftRotAccel(rotateThrust);
};

/* Attack
-------------------- */

// Event

const EventType ShipEventHandler::FireEvent::TYPE;
ShipEventHandler::FireEvent::FireEvent() : Event(TYPE) {}
void ShipEventHandler::FireEventEmitter::emit(std::queue<Event::Ptr>& events) {
    events.push(FireEvent::Ptr(new FireEvent()));
}

// Action

void ShipEventHandler::fire() {
    eventEmitter()->enqueue(objectEventFactory()->createObject(
        BulletSpec::UPtr(new BulletSpec(
            physModel()->pos(), physModel()->rot(), bulletImage
        ))
    ));
}

void ShipEventHandler::purchaseUpgrade(const Upgrade& upgrade) {
    upgradeTree().purchaseUpgrade(upgrade);
}

/* Ship
-------------------------------------------------- */

Ship::Ship(ShipSpec::Ptr spec) :
    HasEventHandlerOf(ShipEventHandler::UPtr(new ShipEventHandler(spec))),
    HasEventEmitterOf(BufferedEventEmitter::UPtr(new BufferedEventEmitter())),
    HasPhysOf(NewtonianPhysModel::UPtr(new NewtonianPhysModel(spec->pos, Vector2D(0, 0), spec->rot, 0.0f))),
    HasGraphicsOf(ImageGraphicsModel::UPtr(new ImageGraphicsModel(spec->image))),
    HasUpgradeTree(ShipUpgrade::SHIP),
    HasUIOf(UpgradeTreeUI::UPtr(new UpgradeTreeUI()))
{
    // Thread dependencies
    trackPhysObserver(graphicsModelWPtr());
    trackPhysObserver(eventHandlerWPtr());
    trackEventEmitterObserver(eventHandlerWPtr());
    trackUpgradeTreeObserver(eventHandlerWPtr());
    trackUpgradeTreeObserver(uiModelWPtr());

    // Virtual method initialisation
    initUpgradeTree();
}

const ObjectFactory Ship::factory = [](ObjectSpec::UPtr spec) {
    return GameObject::Ptr(new Ship(static_unique_pointer_cast<ShipSpec>(move(spec))));
};

Ship::~Ship() {}

void Ship::setObjectEventFactory(ObjectEventFactory::Ptr objectEventFactory) {
    ObjectEventCreator::setObjectEventFactory(objectEventFactory);
    eventHandler().setObjectEventFactory(objectEventFactory);
}

// Organise the available upgrades into a tree.
void Ship::buildUpgradeTree(UpgradeTree& upgradeTree) {
    // Formatted the same as tree itself for ease of reading
    const auto& loadOptimisation = upgradeTree.addUpgrade(ShipUpgrade::LOAD_OPTIMISATION);
    upgradeTree.addUpgrade(loadOptimisation, ShipUpgrade::SPACIAL_COMPRESSION);
    const auto& cooperation = upgradeTree.addUpgrade(loadOptimisation, ShipUpgrade::COOPERATION);
    upgradeTree.addUpgrade(cooperation, ShipUpgrade::OPTIMAL_SELECTION);

    const auto& rearThrusters = upgradeTree.addUpgrade(ShipUpgrade::REAR_THRUSTERS);
    upgradeTree.addUpgrade(rearThrusters, ShipUpgrade::FRONT_THRUSTERS);
    const auto& overdrive = upgradeTree.addUpgrade(rearThrusters, ShipUpgrade::OVERDRIVE);
    upgradeTree.addUpgrade(overdrive, ShipUpgrade::HYPER_JUMP);

    const auto& heavyShells = upgradeTree.addUpgrade(ShipUpgrade::HEAVY_SHELLS);
    upgradeTree.addUpgrade(heavyShells, ShipUpgrade::IONIC_SHELLS);
    const auto& frontAutoCannons = upgradeTree.addUpgrade(heavyShells, ShipUpgrade::FRONT_AUTO_CANNONS);
    upgradeTree.addUpgrade(frontAutoCannons, ShipUpgrade::REAR_AUTO_CANNONS);

    const auto& workerDrone = upgradeTree.addUpgrade(ShipUpgrade::WORKER_DRONE);
    upgradeTree.addUpgrade(workerDrone, ShipUpgrade::ARMOURED_DRONE);
    const auto& mine = upgradeTree.addUpgrade(workerDrone, ShipUpgrade::MINE);
    upgradeTree.addUpgrade(mine, ShipUpgrade::FIGHTER_DRONE);
}

void Ship::beforeFrame() {
    physModel().setAccel(Vector2D(0.0f, 0.0f));
    physModel().setRotAccel(0.0f);
}
