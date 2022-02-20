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
    ship.physModel().shiftAccel(ship.physModel().rot() * ship.engineThrust);
};

Ship::Action Ship::TURN_LEFT_THRUST = [](Ship& ship) {
    ship.physModel().shiftRotAccel(-ship.rotateThrust);
};

Ship::Action Ship::TURN_RIGHT_THRUST = [](Ship& ship) {
    ship.physModel().shiftRotAccel(ship.rotateThrust);
};

/* Upgrade Action
-------------------- */

// Create an action to purchase the given upgrade.
Ship::UpgradeAction::UpgradeAction(const Upgrade& upgrade) : upgrade(upgrade) {}

// The set of all UpgradeActions.
std::map<Ship::Upgrade, std::shared_ptr<Ship::UpgradeAction>> Ship::UpgradeAction::allUpgradeActions = std::map<Ship::Upgrade, std::shared_ptr<Ship::UpgradeAction>>();

// Create an action to purchase the given upgrade. Memoised / strong-cached.
std::shared_ptr<Ship::UpgradeAction> Ship::UpgradeAction::create(const Upgrade& upgrade) {
    const std::map<Upgrade, std::shared_ptr<Ship::UpgradeAction>>::iterator existingAction = allUpgradeActions.find(upgrade);
    if (existingAction != allUpgradeActions.end()) {
        return existingAction->second;
    }

    std::shared_ptr<Ship::UpgradeAction> newAction = std::shared_ptr<Ship::UpgradeAction>(new UpgradeAction(upgrade));
    allUpgradeActions.insert({ upgrade, newAction });
    return newAction;
}

// Try to add the upgrade this action is for.
void Ship::UpgradeAction::operator() (Ship& ship) const {
    // Get upgrade
    std::optional<Node<PurchasableUpgrade>::NodePtr> maybeUpgradeNode = findUpgrade(ship.upgradeTree, upgrade);
    if (!maybeUpgradeNode) return; // Cannot purchase upgrade not in the tree
    Node<PurchasableUpgrade>::NodePtr upgradeNode = maybeUpgradeNode.value();

    // Check parent
    bool canPurchase = false;
    std::optional<Node<PurchasableUpgrade>::NodePtr> parentUpgradeNode = findParentUpgrade(ship.upgradeTree, upgrade);
    // Note: parentUpgradeNode != std::nullopt, as we know the node exists.
    if (
        parentUpgradeNode.value() &&                      // If there is a parent node (you can always purchase the root node, which has no parent)
        !parentUpgradeNode.value()->getValue()->purchased // Parent node isn't yet purchased
    ) {
        return; // Cannot purchase upgrade with unmet dependencies
    }

    // Purchase upgrade
    upgradeNode->getValue()->purchased = true;
}

/* Utils
-------------------------------------------------- */

Node<Ship::PurchasableUpgrade>::NodePtr Ship::addUpgrade(Node<PurchasableUpgrade>::NodePtr parent, Upgrade upgrade) {
    return parent->addChild(new PurchasableUpgrade{ upgrade, false });
}

std::optional<Node<Ship::PurchasableUpgrade>::NodePtr> Ship::findUpgrade(Node<PurchasableUpgrade>::NodePtr root, Upgrade upgrade) {
    return Node<PurchasableUpgrade>::find<NestedUpgradeComparator>(root, new PurchasableUpgrade{ upgrade, false });
}

std::optional<Node<Ship::PurchasableUpgrade>::NodePtr> Ship::findParentUpgrade(Node<PurchasableUpgrade>::NodePtr root, Upgrade upgrade) {
    return Node<PurchasableUpgrade>::findParent<NestedUpgradeComparator>(root, new PurchasableUpgrade{ upgrade, false }); // false is ignored
}

std::wstring Ship::strDump(Node<PurchasableUpgrade>::NodePtr node = nullptr, int indent = 0) const {
    if (!node) node = upgradeTree;

    // Open
    std::wstring upgradeName = std::to_wstring((int)node->getValue()->upgrade);
    std::wstring purchased = node->getValue()->purchased ? L"Purchased" : L"Not Purchased";
    std::wstring ret = upgradeName + L" (" + purchased + L") {";

    // Children
    auto children = node->getChildren();
    if (!children.empty()) ret += L"\n";
    for (const Node<PurchasableUpgrade>::NodePtr& subNode : children) {
        ret += strDump(subNode);
    }
    if (!children.empty()) ret += L"\n";

    // Close
    ret += L"},\n";

    // Return
    return ret;
}

/* Lifecycle
-------------------------------------------------- */

/* Lifecycle
-------------------- */

Ship::Ship(Vector2D pos, Vector2D rot, PictureIndex image)
    : GameObject(
        std::shared_ptr<NoAI<Action>>(new NoAI<Action>()),
        std::shared_ptr<NewtonianPhysModel>(new NewtonianPhysModel(pos, Vector2D(0, 0), rot, 0.0f))
    ),
    image(image),
    upgradeTree(buildUpgradeTree()),
    engineThrust(0.1f),
    rotateThrust(0.01f * RPS) {
}

Node<Ship::PurchasableUpgrade>::NodePtr Ship::buildUpgradeTree() {
    // Formatted the same as tree itself for ease of reading
    auto upgradeTree = Node<PurchasableUpgrade>::create(
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

Ship::~Ship() {}

void Ship::beforeActions() {
    physModel().setAccel(Vector2D(0.0f, 0.0f));
    physModel().setRotAccel(0.0f);
}
void Ship::actions() {
    for (Action* action : actionSource().getActions()) {
        (*action)(*this); // I know, it's ugly. But it's only one line.
    }
}

void Ship::phys() {
    physModel().run();
}

void Ship::draw() {
    MyDrawEngine* graphics = MyDrawEngine::GetInstance();
    graphics->DrawAt(physModel().pos(), image, 1.0f, physModel().rot().angle(), 0.0f);
    MyDrawEngine::GetInstance()->WriteText(Vector2D(-1000, 700), strDump(upgradeTree).c_str(), MyDrawEngine::CYAN);
}
