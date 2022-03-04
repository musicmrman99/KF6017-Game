#include "UpgradeTree.h"

/* Upgrade
-------------------------------------------------- */

Upgrade::Upgrade(const std::wstring name) : name(name) {}

const std::wstring& Upgrade::getName() const { return name; }

/* Purchasable Upgrade
-------------------------------------------------- */

// An upgrade with a 'purchased' boolean flag.
bool UpgradeTree::PurchasableUpgrade::operator== (const PurchasableUpgrade& other) const {
    return upgrade == other.upgrade && purchased == other.purchased;
}

// A comparator for just the upgrade of a purchasable upgrade, ignoring whether it's purchased.
struct UpgradeTree::NestedUpgradeComparator {
    bool operator() (const PurchasableUpgrade& a, const PurchasableUpgrade& b) const {
        return a.upgrade == b.upgrade;
    }
};

/* Upgrade Event Type
-------------------------------------------------- */

UpgradeEventType::UpgradeEventType(const Upgrade& upgrade) : upgrade(upgrade) {}

const EventTypePtr UpgradeEventType::UPGRADE = EventTypeManager::registerNewType()->getValue();

// A comparator for just the upgrade of a purchasable upgrade, ignoring whether it's purchased.
template <class T>
struct ReferenceWrapperLess {
    bool operator() (const std::reference_wrapper<T>& a, const std::reference_wrapper<T>& b) const {
        return a.get() < b.get();
    }
};

using UpgradeKey = std::reference_wrapper<const Upgrade>;
using UpgradeEventTypeMap = std::map<UpgradeKey, const EventTypePtr, ReferenceWrapperLess<const Upgrade>>;

// Create an event type for purchasing the given upgrade. Memoised / strong-cached.
const EventTypePtr& UpgradeEventType::of(const Upgrade& upgrade) {
    // The set of all UpgradeEventTypes
    static UpgradeEventTypeMap allUpgradeEventTypes = UpgradeEventTypeMap();

    // If exists, return
    const UpgradeEventTypeMap::iterator existingEventType = allUpgradeEventTypes.find(UpgradeKey(upgrade));
    if (existingEventType != allUpgradeEventTypes.end()) {
        return existingEventType->second;
    }

    // If not, create and return
    EventTypePtr newAction = EventTypeManager::registerNewType(new UpgradeEventType(upgrade), UPGRADE)->getValue();
    allUpgradeEventTypes.insert({ upgrade, newAction });
    return allUpgradeEventTypes.find(upgrade)->second; // Return a reference to the one in the full list, not the one on the stack
}

/* Upgrade Tree
-------------------------------------------------- */

UpgradeTree::UpgradeTree(const Upgrade& rootUpgrade)
    : upgradeTree(Node<PurchasableUpgrade>::create(new PurchasableUpgrade { rootUpgrade, true})) {
}

// Try to add the upgrade this action is for.
void UpgradeTree::purchaseUpgrade(const Upgrade& upgrade) {
    // Get upgrade
    std::optional<NodePtr> maybeUpgradeNode = findUpgrade(upgrade);
    if (!maybeUpgradeNode) return; // Cannot purchase upgrade not in the tree
    NodePtr upgradeNode = maybeUpgradeNode.value();

    // Check parent
    bool canPurchase = false;
    std::optional<NodePtr> parentUpgradeNode = findParentUpgrade(upgrade);
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

/* Get, Add, and Search
-------------------- */

const UpgradeTree::NodePtr& UpgradeTree::getRootUpgrade() const {
    return upgradeTree;
}

auto UpgradeTree::addUpgrade(const NodePtr& parent, const Upgrade& upgrade) -> NodePtr {
    return parent->addChild(new PurchasableUpgrade { upgrade, false });
}

auto UpgradeTree::addUpgrade(const Upgrade& upgrade) -> NodePtr {
    return addUpgrade(upgradeTree, upgrade);
}

auto UpgradeTree::findUpgrade(const Upgrade& upgrade) -> std::optional<NodePtr> {
    return Node<PurchasableUpgrade>::template find<NestedUpgradeComparator>(
        upgradeTree,
        new PurchasableUpgrade { upgrade, false } // false is ignored
    );
}

auto UpgradeTree::findParentUpgrade(const Upgrade& upgrade) -> std::optional<NodePtr> {
    return Node<PurchasableUpgrade>::template findParent<NestedUpgradeComparator>(
        upgradeTree,
        new PurchasableUpgrade { upgrade, false } // false is ignored
    );
}
