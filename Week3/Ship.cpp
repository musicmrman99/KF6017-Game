#include "Ship.h"

#include <vector>
#include <functional>

#define _USE_MATH_DEFINES
#include <math.h>

#define FPS 60
const float RPS = 2 * M_PI / FPS;

/* Upgrades
-------------------------------------------------- */

bool Ship::PurchasableUpgrade::operator==(const PurchasableUpgrade& other) const {
    return upgrade == other.upgrade && purchased == other.purchased;
}

// A comparator for just the upgrade, ignoring whether it's purchased.
struct NestedUpgradeComparator {
    bool operator() (const Ship::PurchasableUpgrade& a, const Ship::PurchasableUpgrade& b) const {
        return a.upgrade == b.upgrade;
    }
};

/* Actions
-------------------------------------------------- */

void Ship::MainThrustAction::perform(Ship* ship) const {
    ship->accel += ship->rot * ship->engineThrust;
}

void Ship::TurnLeftThrustAction::perform(Ship* ship) const {
    ship->rotVel -= ship->rotateThrust;
}

void Ship::TurnRightThrustAction::perform(Ship* ship) const {
    ship->rotVel += ship->rotateThrust;
}

void Ship::UpgradeAction::perform(Ship* ship) const {
    Node<PurchasableUpgrade>* parentUpgradeNode = findParentUpgrade(ship->upgradeTree, upgrade);
    if (parentUpgradeNode->getValue()->purchased) {
        Node<PurchasableUpgrade>* upgradeNode = findUpgrade(parentUpgradeNode, upgrade);
        upgradeNode->setValue(new PurchasableUpgrade{ upgrade, true });
    }
}

/* Lifecycle
-------------------------------------------------- */

// Action Source

void Ship::setActionSource(ActionSource<Action>* actionSource) {
    this->actionSource = actionSource;
}

// Utils

Node<Ship::PurchasableUpgrade>* Ship::addUpgrade(Node<PurchasableUpgrade>* parent, Upgrade upgrade) {
    return parent->addChild(new PurchasableUpgrade{ upgrade, false });
}

Node<Ship::PurchasableUpgrade>* Ship::findUpgrade(Node<PurchasableUpgrade>* root, Upgrade upgrade) {
    PurchasableUpgrade wrapUpgrade{ upgrade, false }; // false is ignored
    return root->find<NestedUpgradeComparator>(&wrapUpgrade);
}

Node<Ship::PurchasableUpgrade>* Ship::findParentUpgrade(Node<PurchasableUpgrade>* root, Upgrade upgrade) {
    PurchasableUpgrade wrapUpgrade{ upgrade, false }; // false is ignored
    return root->findParent<NestedUpgradeComparator>(&wrapUpgrade);
}

// Lifecycle

Ship::Ship(Vector2D pos, Vector2D rot, PictureIndex image)
    : pos(pos), rot(rot), image(image), actionSource(nullptr)
{
    vel.set(0, 0);
    accel.set(0, 0);
    rotVel = 0.0f;

    engineThrust = 0.1;
    rotateThrust = 0.3 * RPS;

    // Formatted the same as tree itself is structured for ease of reading
    upgradeTree = new Tree<PurchasableUpgrade>(
        new PurchasableUpgrade{ Upgrade::SHIP, true }
    );
    auto loadOptimisation = addUpgrade(upgradeTree, Upgrade::LOAD_OPTIMISATION);
        addUpgrade(loadOptimisation, Upgrade::SPACIAL_COMPRESSION);
        auto cooperation = addUpgrade(loadOptimisation, Upgrade::COOPERATION);
            addUpgrade(cooperation, Upgrade::OPTIMAL_SELECTION);
                
    auto rearThrusters = addUpgrade(upgradeTree, Upgrade::REAR_THRUSTERS);
        addUpgrade(rearThrusters, Upgrade::FRONT_THRUSTERS);
        auto overdrive = addUpgrade(rearThrusters, Upgrade::OVERDRIVE);
            addUpgrade(overdrive, Upgrade::HYPER_JUMP);
                
    auto heavyShells = addUpgrade(upgradeTree, Upgrade::HEAVY_SHELLS);
        addUpgrade(heavyShells, Upgrade::IONIC_SHELLS);
        auto frontAutoCannons = addUpgrade(heavyShells, Upgrade::FRONT_AUTO_CANNONS);
            addUpgrade(frontAutoCannons, Upgrade::REAR_AUTO_CANNONS);
                
    auto workerDrone = addUpgrade(upgradeTree, Upgrade::WORKER_DRONE);
        addUpgrade(workerDrone, Upgrade::ARMOURED_DRONE);
        auto mine = addUpgrade(workerDrone, Upgrade::MINE);
            addUpgrade(mine, Upgrade::FIGHTER_DRONE);
}

Ship::~Ship() {
    delete upgradeTree;
    upgradeTree = nullptr;
}

void Ship::beforeActions() {
    accel.set(0.0f, 0.0f);
    rotVel = 0.0f;
}
void Ship::actions() {
    for (Action* action : actionSource->getActions()) {
        action->perform(this);
    }
}

void Ship::beforePhys() {}
void Ship::phys() {
    rot.setBearing(rot.angle() + rotVel, 1);

    vel += accel;
    pos += vel;
}

void Ship::beforeDraw() {}
void Ship::draw() {
    MyDrawEngine* graphics = MyDrawEngine::GetInstance();
    graphics->DrawAt(pos, image, 1.0f, rot.angle(), 0.0f);
}

void Ship::afterFrame() {}
