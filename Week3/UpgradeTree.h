#pragma once

#include <string>

#include "Event.h"
#include "Symbol.h"

// An upgrade
class Upgrade final : public Symbol {
private:
	const std::wstring name;

public:
	Upgrade(const std::wstring name);

	const std::wstring& getName() const;
};

// A template for the event types for the type of upgrade.
class UpgradeEventType final : public EventType {
private:
	UpgradeEventType(const Upgrade& upgrade);

public:
	// The upgrade this type of event will lead to attempting to purchase.
	const Upgrade& upgrade;

	// The root event type of all events of the UpgradeEventType.
	static const EventCategory::Ptr UPGRADE;

	// Memoised factory for upgrade event types.
	static const EventType::Ptr& of(const Upgrade& upgrade);
};

// A tree of upgrades
class UpgradeTree final {
private:
	// An upgrade and whether it has been purchased for this ship.
	struct PurchasableUpgrade {
		const Upgrade& upgrade;
		bool purchased;

		bool operator== (const PurchasableUpgrade& other) const;
	};

	// A comparator for PurchasableUpgrades that ignores whether they are purchased.
	struct NestedUpgradeComparator;

public:
	using NodePtr = Node<PurchasableUpgrade>::NodePtr;

private:
	NodePtr upgradeTree;

public:
	// Constructor
	UpgradeTree(const Upgrade& rootUpgrade);

	// Get, Set, and Find
	const NodePtr& getRootUpgrade() const;

	NodePtr addUpgrade(const NodePtr& parent, const Upgrade& upgrade);
	NodePtr addUpgrade(const Upgrade& upgrade);

	std::optional<NodePtr> findUpgrade(const Upgrade& upgrade);
	std::optional<NodePtr> findParentUpgrade(const Upgrade& upgrade);

	// Attempt to purchase the given upgrade.
	void purchaseUpgrade(const Upgrade& upgrade);
};
