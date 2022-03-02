#include "Ship.h"

#include <vector>
#include <functional>
#include <string>

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

const EventTypePtr Ship::MAIN_THRUST = EventTypeManager::create()->getValue();
const EventTypePtr Ship::TURN_LEFT_THRUST = EventTypeManager::create()->getValue();
const EventTypePtr Ship::TURN_RIGHT_THRUST = EventTypeManager::create()->getValue();

void Ship::mainThrust() {
    physModel().shiftAccel(physModel().rot() * physModel().toDUPS(engineThrust));
};

void Ship::turnLeftThrust() {
    physModel().shiftRotAccel(-physModel().toRPS(rotateThrust));
};

void Ship::turnRightThrust() {
    physModel().shiftRotAccel(physModel().toRPS(rotateThrust));
};

/* Upgrade Action (and Event Type)
-------------------- */

const EventTypePtr Ship::UPGRADE = EventTypeManager::create()->getValue();

Ship::UpgradeEventType::UpgradeEventType(const Upgrade& upgrade) : upgrade(upgrade) {}

// Create an event type for purchasing the given upgrade. Memoised / strong-cached.
const EventTypePtr& Ship::UpgradeEventType::get(const Upgrade& upgrade) {
    // The set of all UpgradeEventTypes
    static std::map<Upgrade, const EventTypePtr> allUpgradeEventTypes = std::map<Upgrade, const EventTypePtr>();

    // If exists, return
    const std::map<Upgrade, const EventTypePtr>::iterator existingEventType = allUpgradeEventTypes.find(upgrade);
    if (existingEventType != allUpgradeEventTypes.end()) {
        return existingEventType->second;
    }

    // If not, create and return
    EventTypePtr newAction = EventTypeManager::create(UPGRADE)->getValue();
    allUpgradeEventTypes.insert({ upgrade, newAction });
    return allUpgradeEventTypes.find(upgrade)->second; // Return a reference to the one in the full list, not the one on the stack
}

// Try to add the upgrade this action is for.
void Ship::purchaseUpgrade(const Upgrade& upgrade) {
    // Get upgrade
    std::optional<Node<PurchasableUpgrade>::NodePtr> maybeUpgradeNode = findUpgrade(upgradeTree, upgrade);
    if (!maybeUpgradeNode) return; // Cannot purchase upgrade not in the tree
    Node<PurchasableUpgrade>::NodePtr upgradeNode = maybeUpgradeNode.value();

    // Check parent
    bool canPurchase = false;
    std::optional<Node<PurchasableUpgrade>::NodePtr> parentUpgradeNode = findParentUpgrade(upgradeTree, upgrade);
    // Note: parentUpgradeNode != std::nullopt, as we know the node exists.
    if (
        parentUpgradeNode.value() &&                      // If there is a parent node (you can always purchase the root node, which has no parent)
        !parentUpgradeNode.value()->getValue()->purchased // and it isn't yet purchased
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
        std::shared_ptr<NullEventEmitter>(new NullEventEmitter()),
        std::shared_ptr<NewtonianPhysModel>(new NewtonianPhysModel(pos, Vector2D(0, 0), rot, 0.0f))
    ),
    image(image),
    upgradeTree(buildUpgradeTree()),
    engineThrust(0.2f), // Distance units / second^2
    rotateThrust(0.01f) // Revolutions / second^2
{}

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

void Ship::handle(const Event& e) {
         if (EventTypeManager::isOfType(e.type, MAIN_THRUST)) mainThrust();
    else if (EventTypeManager::isOfType(e.type, TURN_LEFT_THRUST)) turnLeftThrust();
    else if (EventTypeManager::isOfType(e.type, TURN_RIGHT_THRUST)) turnRightThrust();
    
    else if (EventTypeManager::isOfType(e.type, UPGRADE)) purchaseUpgrade(static_cast<UpgradeEventType*>(e.type.get())->upgrade);
}

void Ship::beforeActions() {
    physModel().setAccel(Vector2D(0.0f, 0.0f));
    physModel().setRotAccel(0.0f);
}
void Ship::draw() {
    MyDrawEngine* graphics = MyDrawEngine::GetInstance();
    graphics->DrawAt(physModel().pos(), image, 1.0f, physModel().rot().angle(), 0.0f);
    MyDrawEngine::GetInstance()->WriteText(Vector2D(-1000, 700), strDump(upgradeTree).c_str(), MyDrawEngine::CYAN);
}
