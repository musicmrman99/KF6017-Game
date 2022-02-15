#include "Ship.h"

#include <vector>
#include <functional>
#include <string>

#define _USE_MATH_DEFINES
#include <math.h>

#define FPS 60.0f
const float RPS = 2 * (float) M_PI / FPS;

/* Upgrades
-------------------------------------------------- */

bool Ship::PurchasableUpgrade::operator==(const PurchasableUpgrade& other) const {
    return upgrade == other.upgrade && purchased == other.purchased;
}

// A comparator for just the upgrade of a purchasable upgrade, ignoring whether it's purchased.
struct NestedUpgradeComparator {
    bool operator() (const Ship::PurchasableUpgrade& a, const Ship::PurchasableUpgrade& b) const {
        return a.upgrade == b.upgrade;
    }
};

/* Actions
-------------------------------------------------- */

/* Movement Actions
-------------------- */

Ship::Action Ship::MAIN_THRUST = [](Ship& ship) {
    ship.physModel.shiftAccel(ship.physModel.rot() * ship.engineThrust);
};

Ship::Action Ship::TURN_LEFT_THRUST = [](Ship& ship) {
    ship.physModel.shiftRotAccel(-ship.rotateThrust);
};

Ship::Action Ship::TURN_RIGHT_THRUST = [](Ship& ship) {
    ship.physModel.shiftRotAccel(ship.rotateThrust);
};

/* Upgrade Action
-------------------- */

// Create an action to purchase the given upgrade.
Ship::UpgradeAction::UpgradeAction(const Upgrade& upgrade) : upgrade(upgrade) {}

// The set of all UpgradeActions.
std::map<Ship::Upgrade, Ship::UpgradeAction*> Ship::UpgradeAction::allUpgradeActions = std::map<Ship::Upgrade, Ship::UpgradeAction*>();

// Create an action to purchase the given upgrade. Memoised.
Ship::UpgradeAction* Ship::UpgradeAction::create(const Upgrade& upgrade) {
    const std::map<Upgrade, UpgradeAction*>::iterator existingAction = allUpgradeActions.find(upgrade);
    if (existingAction != allUpgradeActions.end()) {
        return existingAction->second;
    }

    UpgradeAction* newAction = new UpgradeAction(upgrade);
    allUpgradeActions.insert({ upgrade, newAction });
    return newAction;
}

// Delete all UpgradeAction instances.
void Ship::UpgradeAction::deleteAll() {
    for (std::pair<const Upgrade, UpgradeAction*>& upgradeAction : allUpgradeActions) {
        delete upgradeAction.second;
        upgradeAction.second = nullptr;
    }
    allUpgradeActions.clear();
}

// Try to add the upgrade this action is for.
void Ship::UpgradeAction::operator() (Ship& ship) const {
    // Warning: Assumes all declared upgrades (in Ship::Upgrade) are in the tree
    Node<PurchasableUpgrade>* parentUpgradeNode = findParentUpgrade(ship.upgradeTree, upgrade);
    if (!parentUpgradeNode || parentUpgradeNode->getValue()->purchased) {
        Node<PurchasableUpgrade>* upgradeNode = findUpgrade(parentUpgradeNode, upgrade);
        upgradeNode->setValue(new PurchasableUpgrade{ upgrade, true });
    }
}

/* Utils
-------------------------------------------------- */

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

std::wstring Ship::strDump(Node<PurchasableUpgrade>* node = nullptr, int indent = 0) const {
    if (!node) node = upgradeTree;

    std::wstring ret = std::to_wstring((int)node->getValue()->upgrade) + L" - " + std::to_wstring(node->getValue()->purchased) + L" {";
    auto children = node->getChildren();
    if (!children.empty()) ret += L"\n";
    for (Node<PurchasableUpgrade>* subNode : children) {
        ret += strDump(subNode);
    }
    if (!children.empty()) ret += L"\n";
    ret += L"},\n";
    return ret;
}

/* Lifecycle
-------------------------------------------------- */

/* Action Source
-------------------- */

void Ship::setActionSource(ActionSource<Action>* actionSource) {
    this->actionSource = actionSource;
}

/* Lifecycle
-------------------- */

Ship::Ship(Vector2D pos, Vector2D rot, PictureIndex image)
    : physModel(NewtonianPhysModel(pos, Vector2D(0, 0), rot, 0.0f)),
    image(image),
    actionSource(nullptr),
    upgradeTree(buildUpgradeTree()),
    engineThrust(0.1f),
    rotateThrust(0.01f * RPS) {
}

Tree<Ship::PurchasableUpgrade>* Ship::buildUpgradeTree() {
    // Formatted the same as tree itself for ease of reading
    Tree<Ship::PurchasableUpgrade>* upgradeTree = new Tree<PurchasableUpgrade>(
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

    return upgradeTree;
}

Ship::~Ship() {
    delete upgradeTree;
    upgradeTree = nullptr;
}

void Ship::beforeActions() {
    physModel.setAccel(Vector2D(0.0f, 0.0f));
    physModel.setRotAccel(0.0f);
}
void Ship::actions() {
    for (Action* action : actionSource->getActions()) {
        (*action)(*this); // I know, it's ugly. But it's only one line.
    }
}

void Ship::beforePhys() {}
void Ship::phys() {
    physModel.run();
}

void Ship::beforeDraw() {}
void Ship::draw() {
    MyDrawEngine* graphics = MyDrawEngine::GetInstance();
    graphics->DrawAt(physModel.pos(), image, 1.0f, physModel.rot().angle(), 0.0f);
    MyDrawEngine::GetInstance()->WriteText(Vector2D(-1000, 700), strDump(upgradeTree).c_str(), MyDrawEngine::CYAN);
}

void Ship::afterFrame() {}
