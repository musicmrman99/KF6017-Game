#pragma once

#include <vector>

#include "mydrawengine.h"
#include "myinputs.h"

#include "Node.h"
#include "ActionSource.h"

class Ship {
public:
	// Game Actions
	/*
	Design Reasoning:

	An ActionSource provides Actions of the appropriate type (Ship::Action) for this type (Ship)
	that it requests a particular instance of this type (a Ship) to perform - without any knowledge
	of which type of object it is providing actions for. This class can provide its supported actions
	(to be bound to a KeyMap/ActionSource elsewhere) either as data, or as executable code. It must be
	parameterisable, eg. for purchasing a particular upgrade. It can be defined as data (a class; enums
	don't support the parameterised command pattern), or as a polymorphic function object. If data, we
	would have to parse the type of command, then manually dispatch to the right method based on the
	data. We would rather automatically dynamically dispatch using C++'s type system. However, to access
	a particular ship's data (remember, the Actions are instance-independent by design) requires either
	binding the Action objects to a particular ship (which causes a bind cycle - a ship is bound to a
	keymap, whose actions are bound to the ship - which is bad design), or ...

	Couple the Actions to the Ship class (as they are actions any Ship can perform), rather than a
	particular instance, requiring the instance (ship) to which the action is to be applied to be
	passed to the Action.
	*/
	class Action {
	public:
		virtual void perform(Ship* ship) const = 0;
	};

	// Movement
	class MainThrustAction : public Action {
	public:
		virtual void perform(Ship* ship) const;
	};
	class TurnLeftThrustAction : public Action {
	public:
		virtual void perform(Ship* ship) const;
	};
	class TurnRightThrustAction : public Action {
	public:
		virtual void perform(Ship* ship) const;
	};

	// Upgrades
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
	struct PurchasableUpgrade {
		Upgrade upgrade;
		bool purchased;

		bool operator== (const PurchasableUpgrade& other) const;
	};

	class UpgradeAction : public Action {
	private:
		const Upgrade upgrade;

	public:
		UpgradeAction(const Upgrade upgrade) : upgrade(upgrade) {}
		virtual void perform(Ship* ship) const;
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
