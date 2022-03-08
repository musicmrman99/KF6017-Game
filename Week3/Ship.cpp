#include "Ship.h"

#include <string>

#include "QueueUtils.h"

#include "UpgradeTreeUI.h"
#include "Bullet.h"

/* Get/Set the right types
-------------------------------------------------- */

NewtonianPhysModel& Ship::physModel() {
    return static_cast<NewtonianPhysModel&>(GameObject::physModel());
}
void Ship::setPhysModel(PhysModelPtr physModel) {
    if (physModel && dynamic_cast<NewtonianPhysModel*>(physModel.get())) {
        GameObject::setPhysModel(physModel);
    }
}

/* Actions
-------------------------------------------------- */

/* Movement Actions
-------------------- */

const EventType::Ptr Ship::MAIN_THRUST = EventTypeManager::registerNewType();
const EventType::Ptr Ship::TURN_LEFT_THRUST = EventTypeManager::registerNewType();
const EventType::Ptr Ship::TURN_RIGHT_THRUST = EventTypeManager::registerNewType();

void Ship::mainThrust() {
    physModel().shiftAccel(physModel().rot() * physModel().toDUPS(engineThrust));
};

void Ship::turnLeftThrust() {
    physModel().shiftRotAccel(-physModel().toRPS(rotateThrust));
};

void Ship::turnRightThrust() {
    physModel().shiftRotAccel(physModel().toRPS(rotateThrust));
};

/* Attack
-------------------- */

const EventType::Ptr Ship::FIRE = EventTypeManager::registerNewType();

void Ship::fire() {
    Bullet::UPtr bullet = Bullet::UPtr(new Bullet(physModel().pos(), physModel().rot(), bulletImage));
    globalEventBuffer.push(ReleaseObjectEvent::Ptr(new ReleaseObjectEvent(objectManager, move(bullet))));
}

/* Upgrades
-------------------- */

const Upgrade Ship::SHIP(L"Ship");

const Upgrade Ship::LOAD_OPTIMISATION(L"Load Optimisation");
const Upgrade Ship::SPACIAL_COMPRESSION(L"Spacial Compression");
const Upgrade Ship::COOPERATION(L"Cooperation");
const Upgrade Ship::OPTIMAL_SELECTION(L"Optimal Selection");

const Upgrade Ship::FRONT_THRUSTERS(L"Front Thrusters");
const Upgrade Ship::REAR_THRUSTERS(L"Rear Thrusters");
const Upgrade Ship::OVERDRIVE(L"Overdrive");
const Upgrade Ship::HYPER_JUMP(L"Hyper Jump");

const Upgrade Ship::HEAVY_SHELLS(L"Heavy Shells");
const Upgrade Ship::FRONT_AUTO_CANNONS(L"Front Auto-Cannons");
const Upgrade Ship::REAR_AUTO_CANNONS(L"Rear Auto-Cannons");
const Upgrade Ship::IONIC_SHELLS(L"Ionic Shells");

const Upgrade Ship::WORKER_DRONE(L"Worker Drone");
const Upgrade Ship::ARMOURED_DRONE(L"Armoured Drone");
const Upgrade Ship::MINE(L"Mine");
const Upgrade Ship::FIGHTER_DRONE(L"Fighter Drone");

void Ship::buildUpgradeTree() {
    // Formatted the same as tree itself for ease of reading
    const auto& loadOptimisation = upgradeTree.addUpgrade(LOAD_OPTIMISATION);
    upgradeTree.addUpgrade(loadOptimisation, SPACIAL_COMPRESSION);
    const auto& cooperation = upgradeTree.addUpgrade(loadOptimisation, COOPERATION);
    upgradeTree.addUpgrade(cooperation, OPTIMAL_SELECTION);

    const auto& rearThrusters = upgradeTree.addUpgrade(REAR_THRUSTERS);
    upgradeTree.addUpgrade(rearThrusters, FRONT_THRUSTERS);
    const auto& overdrive = upgradeTree.addUpgrade(rearThrusters, OVERDRIVE);
    upgradeTree.addUpgrade(overdrive, HYPER_JUMP);

    const auto& heavyShells = upgradeTree.addUpgrade(HEAVY_SHELLS);
    upgradeTree.addUpgrade(heavyShells, IONIC_SHELLS);
    const auto& frontAutoCannons = upgradeTree.addUpgrade(heavyShells, FRONT_AUTO_CANNONS);
    upgradeTree.addUpgrade(frontAutoCannons, REAR_AUTO_CANNONS);

    const auto& workerDrone = upgradeTree.addUpgrade(WORKER_DRONE);
    upgradeTree.addUpgrade(workerDrone, ARMOURED_DRONE);
    const auto& mine = upgradeTree.addUpgrade(workerDrone, MINE);
    upgradeTree.addUpgrade(mine, FIGHTER_DRONE);
}

// Use UpgradeEventType::of() to get the upgrade event type for these upgrades.

/* Getters
-------------------------------------------------- */

const UpgradeTree& Ship::getUpgradeTree() {
    return upgradeTree;
}

/* Lifecycle
-------------------------------------------------- */

Ship::Ship(
    Vector2D pos, Vector2D rot, PictureIndex image, PictureIndex bulletImage, ObjectManager::Ptr objectManager,
    std::shared_ptr<NewtonianPhysModel> physModel
) : GameObject(
        std::shared_ptr<NullEventEmitter>(new NullEventEmitter()),
        physModel,
        std::shared_ptr<ImageGraphicsModel>(new ImageGraphicsModel(physModel, image)),
        std::shared_ptr<UpgradeTreeUI>(new UpgradeTreeUI(upgradeTree))
    ),
    objectManager(objectManager),
    bulletImage(bulletImage),
    upgradeTree(UpgradeTree(SHIP)),
    engineThrust(0.2f),   // Distance units / second^2
    rotateThrust(0.01f) { // Revolutions / second^2
}

Ship::Ship(Vector2D pos, Vector2D rot, PictureIndex image, PictureIndex bulletImage, ObjectManager::Ptr objectManager)
    : Ship(
        pos, rot, image, bulletImage, objectManager,
        std::shared_ptr<NewtonianPhysModel>(new NewtonianPhysModel(pos, Vector2D(0, 0), rot, 0.0f))
    ) {
    buildUpgradeTree();
}

Ship::~Ship() {}

void Ship::beforeActions() {
    physModel().setAccel(Vector2D(0.0f, 0.0f));
    physModel().setRotAccel(0.0f);
}

void Ship::emit(std::queue<Event::Ptr>& globalEvents) {
    shiftInto(globalEventBuffer, globalEvents);
}

void Ship::handle(const Event::Ptr e) {
         if (EventTypeManager::isOfType(e->type, MAIN_THRUST)) mainThrust();
    else if (EventTypeManager::isOfType(e->type, TURN_LEFT_THRUST)) turnLeftThrust();
    else if (EventTypeManager::isOfType(e->type, TURN_RIGHT_THRUST)) turnRightThrust();

    else if (EventTypeManager::isOfType(e->type, FIRE)) fire();
    
    else if (EventTypeManager::isOfType(e->type, UpgradeEventType::UPGRADE)) {
        upgradeTree.purchaseUpgrade(std::static_pointer_cast<UpgradeEventType>(e->type)->upgrade);
    }
}
