#pragma once

#include <vector>
#include <map>
#include <memory>

#include "Node.h"

#include "MyDrawEngine.h"
#include "MyInputs.h"

#include "GameObject.h"
#include "NewtonianPhysModel.h"
#include "Event.h"

class Ship : public GameObject<NewtonianPhysModel> {
public:
	// Movement
	static const EventTypePtr MAIN_THRUST;
	static const EventTypePtr TURN_LEFT_THRUST;
	static const EventTypePtr TURN_RIGHT_THRUST;

	void mainThrust();
	void turnLeftThrust();
	void turnRightThrust();

	// Upgrades

	// An upgrade that a ship can make.
	enum class Upgrade {
		SHIP,

		LOAD_OPTIMISATION,
		COOPERATION,
		OPTIMAL_SELECTION,
		SPACIAL_COMPRESSION,

		FRONT_THRUSTERS,
		REAR_THRUSTERS,
		OVERDRIVE,
		HYPER_JUMP,

		HEAVY_SHELLS,
		FRONT_AUTO_CANNONS,
		REAR_AUTO_CANNONS,
		IONIC_SHELLS,

		WORKER_DRONE,
		ARMOURED_DRONE,
		MINE,
		FIGHTER_DRONE
	};

	// An upgrade and whether it has been purchased for this ship.
	struct PurchasableUpgrade {
		Upgrade upgrade;
		bool purchased;

		bool operator== (const PurchasableUpgrade& other) const;
	};

	// Upgrade action
	static const EventTypePtr UPGRADE;

	// A template for the event types that upgrade the ship.
	// See Ship::Upgrade.
	class UpgradeEventType : public BaseEventType {
	private:
		UpgradeEventType(const Upgrade& upgrade);

	public:
		const Upgrade upgrade;

		// Memoised factory for upgrade event types.
		static const EventTypePtr& get(const Upgrade& upgrade);
	};

	// Attempt to purchase the given upgrade for this ship.
	void purchaseUpgrade(const Upgrade& upgrade);

private:
	PictureIndex image; // Graphical Representation

	// Gameplay
	Node<PurchasableUpgrade>::NodePtr upgradeTree;
	
	float engineThrust;
	float rotateThrust;

public:
	// Utils
	static Node<PurchasableUpgrade>::NodePtr addUpgrade(Node<PurchasableUpgrade>::NodePtr parent, Upgrade upgrade);
	static std::optional<Node<Ship::PurchasableUpgrade>::NodePtr> findUpgrade(Node<PurchasableUpgrade>::NodePtr root, Upgrade upgrade);
	static std::optional<Node<Ship::PurchasableUpgrade>::NodePtr> findParentUpgrade(Node<PurchasableUpgrade>::NodePtr root, Upgrade upgrade);
	std::wstring strDump(Node<PurchasableUpgrade>::NodePtr node, int indent) const;

	// Lifecycle
	Ship(Vector2D pos, Vector2D rot, PictureIndex image);
	static Node<Ship::PurchasableUpgrade>::NodePtr buildUpgradeTree();
	~Ship();

	virtual void handle(const Event& e) override;

	virtual void beforeActions() override;
	virtual void actions() override;
	virtual void phys() override;
	virtual void draw() override;
};
