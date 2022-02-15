#pragma once

#include <vector>
#include <map>

#include "Node.h"

#include "MyDrawEngine.h"
#include "MyInputs.h"

#include "ActionSource.h"

class Ship {
public:
	/*
	Design Reasoning:

	An ActionSource provides Actions of a type (Ship::Action) that is appropriate for this type (Ship).
	Each instance of this type (Ship) requests Actions (Ship::Actions) from its source, which provides
	those Actions without any knowledge of which object it is providing them to.
	
	Constraint: Actions must be parameterisable (eg. for purchasing a particular upgrade).

	This class could provide its supported actions (to be bound to a KeyMap/AI elsewhere) either as data
	(and define methods that correspond to each action), or as executable code. These would mean either
	defining Action as a class (as enums don't support the parameters / the command pattern), or as a
	polymorphic function type.
	
	If defined as data, Ship would have to parse the type of each action requested and manually dispatch
	to the right method based on the data (after explicit casting, as different kinds of actions contain
	different data). We would rather dynamically dispatch to the right action using C++'s type system, as
	it reduces maintainance burden. This rules out defining an Action as data to be interpreted.
	
	If defined as a function, it would need to access a particular ship's data, which requires either
	binding the Action objects to a particular ship (which causes a bind cycle - a ship is bound to a
	keymap, whose actions are bound to the ship - which is bad design), or defining them to take a Ship
	as a parameter. The latter option was chosen.

	Solution:
	
	Couple the Actions to the Ship class (rather than a particular instance) and require passing the Ship
	to apply the Action to.
	*/

	// An action that can be applied to a ship.
	using Action = const std::function<void (Ship& ship)>;

	// Movement

	static Action MAIN_THRUST;
	static Action TURN_LEFT_THRUST;
	static Action TURN_RIGHT_THRUST;

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

	// A template for an action to upgrade the ship.
	// See Ship::Upgrade.
	class UpgradeAction : public Action {
	private:
		const Upgrade upgrade;
		static std::map<Upgrade, UpgradeAction*> allUpgradeActions;

		UpgradeAction(const Upgrade& upgrade);

	public:
		// Memoised factory for upgrade actions.
		static UpgradeAction* create(const Upgrade& upgrade);
		static void deleteAll();

		virtual void operator() (Ship& ship) const;
	};

private:
	// Graphical Representation
	PictureIndex image;

	// Logical Representation
	Vector2D pos;
	Vector2D vel;
	Vector2D accel;

	Vector2D rot;
	float rotVel;
	float rotAccel;

	// Action Source
	ActionSource<Action>* actionSource;

	// Gameplay Quantities & Upgrades
	Tree<PurchasableUpgrade>* upgradeTree;
	
	float engineThrust;
	float rotateThrust;

public:
	// Action Source
	void setActionSource(ActionSource<Action>* actionSource);

	// Utils
	static Node<PurchasableUpgrade>* addUpgrade(Node<PurchasableUpgrade>* parent, Upgrade upgrade);
	static Node<PurchasableUpgrade>* findUpgrade(Node<PurchasableUpgrade>* root, Upgrade upgrade);
	static Node<PurchasableUpgrade>* findParentUpgrade(Node<PurchasableUpgrade>* root, Upgrade upgrade);
	std::wstring strDump(Node<PurchasableUpgrade>* node, int indent) const;

	// Lifecycle
	Ship(Vector2D pos, Vector2D rot, PictureIndex image);
	~Ship();

	void beforeActions();
	void actions();

	void beforePhys();
	void phys();

	void beforeDraw();
	void draw();

	void afterFrame();
};
