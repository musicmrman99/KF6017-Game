#pragma once

#include <string>

#include "Symbol.h"
#include "Node.h"

#include "Event.h"
#include "EventHandler.h"
#include "EventEmitter.h"

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
	static const EventType TYPE;

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
class UpgradeTree final : public EventHandler {
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
	using Ptr = std::shared_ptr<UpgradeTree>;
	using UPtr = std::unique_ptr<UpgradeTree>;
	using WPtr = std::weak_ptr<UpgradeTree>;

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

	// Handle an event to purchase an upgrade
	virtual void handle(const Event::Ptr e) override;
};
