#include <map>

#include "ReferenceWrapperUtils.h"

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

/* Upgrade Event and Emitter
-------------------------------------------------- */

// Upgrade Event
const EventType UpgradeEvent::TYPE;
UpgradeEvent::UpgradeEvent(const Upgrade& upgrade) : Event(TYPE), upgrade(upgrade) {}

// Upgrade Event Emitter
UpgradeEventEmitter::UpgradeEventEmitter(const Upgrade& upgrade) : upgrade(upgrade) {}
void UpgradeEventEmitter::UpgradeEventEmitter::emit(std::queue<Event::Ptr>& events) {
    events.push(UpgradeEvent::Ptr(new UpgradeEvent(upgrade)));
}

/* Upgrade Tree
-------------------------------------------------- */

UpgradeTree::UpgradeTree(const Upgrade& rootUpgrade)
    : upgradeTree(Node<PurchasableUpgrade>::create(new PurchasableUpgrade { rootUpgrade, true })) {
}

/* Get, Add, Search
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

/* Upgrade and Events
-------------------- */

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

void UpgradeTree::handle(const Event::Ptr e) {
    if (e->type == UpgradeEvent::TYPE) {
        purchaseUpgrade(std::static_pointer_cast<UpgradeEvent>(e)->upgrade);
    }
}
