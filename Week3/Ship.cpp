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
// Can't enforce uniqueness - GameObject interface requires shared_ptr.
void Ship::setPhysModel(PhysModel::Ptr physModel) {
    if (physModel) {
        if (auto newtonianPhysModel = std::dynamic_pointer_cast<NewtonianPhysModel>(physModel)) {
            GameObject::setPhysModel(physModel);
            static_cast<ImageGraphicsModel&>(GameObject::graphicsModel()).setPhysModel(newtonianPhysModel);
        }
    }
}

/* Actions
-------------------------------------------------- */

/* Movement Actions
-------------------- */

void Ship::MainThrustEventEmitter::emit(std::queue<Event::Ptr>& events) {
    events.push(MainThrustEvent::Ptr(new MainThrustEvent()));
}
void Ship::TurnLeftThrustEventEmitter::emit(std::queue<Event::Ptr>& events) {
    events.push(TurnLeftThrustEvent::Ptr(new TurnLeftThrustEvent()));
}
void Ship::TurnRightThrustEventEmitter::emit(std::queue<Event::Ptr>& events) {
    events.push(TurnRightThrustEvent::Ptr(new TurnRightThrustEvent()));
}

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

void Ship::FireEventEmitter::emit(std::queue<Event::Ptr>& events) {
    events.push(FireEvent::Ptr(new FireEvent()));
}

void Ship::fire() {
    Bullet::UPtr bullet = Bullet::UPtr(new Bullet(physModel().pos(), physModel().rot(), bulletImage));
    globalEventBuffer.push(ReleaseObjectEvent::create(objectManager, move(bullet)));
}

/* Upgrades
-------------------- */

// Use the generic UpgradeEvent type / UpgradeEventEmitter for Ship upgrades.

// Define what upgrades are available.
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

// Organise the available upgrades into a tree.
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

/* Getters
-------------------------------------------------- */

const UpgradeTree& Ship::getUpgradeTree() {
    return upgradeTree;
}

/* Lifecycle
-------------------------------------------------- */

Ship::Ship(
    Vector2D pos, Vector2D rot, PictureIndex image, PictureIndex bulletImage, ObjectManager::WPtr objectManager,
    NewtonianPhysModel::Ptr physModel
) : GameObject(
        NullEventEmitter::UPtr(new NullEventEmitter()),
        physModel,
        ImageGraphicsModel::UPtr(new ImageGraphicsModel(physModel, image)),
        UpgradeTreeUI::UPtr(new UpgradeTreeUI(upgradeTree))
    ),
    objectManager(objectManager),
    bulletImage(bulletImage),
    upgradeTree(UpgradeTree(SHIP)),
    engineThrust(0.2f),   // Distance units / second^2
    rotateThrust(0.01f) { // Revolutions / second^2
}

Ship::Ship(Vector2D pos, Vector2D rot, PictureIndex image, PictureIndex bulletImage, ObjectManager::WPtr objectManager)
    : Ship(
        pos, rot, image, bulletImage, objectManager,
        NewtonianPhysModel::UPtr(new NewtonianPhysModel(pos, Vector2D(0, 0), rot, 0.0f))
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
         if (std::dynamic_pointer_cast<MainThrustEvent>(e)) mainThrust();
    else if (std::dynamic_pointer_cast<TurnLeftThrustEvent>(e)) turnLeftThrust();
    else if (std::dynamic_pointer_cast<TurnRightThrustEvent>(e)) turnRightThrust();

    else if (std::dynamic_pointer_cast<FireEvent>(e)) fire();
    
    else if (auto ue = std::dynamic_pointer_cast<UpgradeEvent>(e)) {
        upgradeTree.purchaseUpgrade(ue->upgrade);
    }
}
