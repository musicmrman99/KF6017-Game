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

// A generic upgrade event type.
class UpgradeEvent final : public Event {
public:
	const Upgrade& upgrade;
	UpgradeEvent(const Upgrade& upgrade);
};

// A generic upgrade event emitter.
class UpgradeEventEmitter final : public EventEmitter {
private:
	const Upgrade& upgrade;

public:
	UpgradeEventEmitter(const Upgrade& upgrade);
	virtual void emit(std::queue<Event::Ptr>& events) override;
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
