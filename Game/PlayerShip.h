#pragma once

// Dependencies
#include <memory>
#include "MyDrawEngine.h"

// Traits
#include "Ship.h"
#include "HasEventHandlerOf.h"
#include "HasUIOf.h"
#include "HasUpgradeTree.h"

// Models
#include "UpgradeTreeUI.h"

// Creation
#include "ObjectFactory.h"
#include "PlayerShipSpec.h"

/* Static Definitions
-------------------- */

// Define what upgrades are available.
struct PlayerShipUpgrade {
	static const Upgrade SHIP; // Root Upgrade

	static const Upgrade LOAD_OPTIMISATION;
	static const Upgrade SPACIAL_COMPRESSION;
	static const Upgrade COOPERATION;
	static const Upgrade OPTIMAL_SELECTION;

	static const Upgrade FRONT_THRUSTERS;
	static const Upgrade REAR_THRUSTERS;
	static const Upgrade OVERDRIVE;
	static const Upgrade HYPER_JUMP;

	static const Upgrade HEAVY_SHELLS;
	static const Upgrade FRONT_AUTO_CANNONS;
	static const Upgrade REAR_AUTO_CANNONS;
	static const Upgrade IONIC_SHELLS;

	static const Upgrade WORKER_DRONE;
	static const Upgrade ARMOURED_DRONE;
	static const Upgrade MINE;
	static const Upgrade FIGHTER_DRONE;
};

/* Event Handler
-------------------- */

class PlayerShipEventHandler final :
	public EventHandler,
	public PhysObserverOf<NewtonianPhysModel>,
	public EventEmitterObserverOf<BufferedEventEmitter>,
	public UpgradeTreeObserver,
	public ObjectEventCreator
{
private:
	// Gameplay
	float engineThrust;
	float rotateThrust;

	PictureIndex bulletImage;

public:
	using UPtr = std::unique_ptr<PlayerShipEventHandler>;

	PlayerShipEventHandler(PlayerShipSpec::Ptr spec);

	virtual void handle(const Event::Ptr e) override;

	// Movement

	class MainThrustEvent final : public Event {
	public:
		static const EventType TYPE;
		MainThrustEvent();
	};
	class TurnLeftThrustEvent final : public Event {
	public:
		static const EventType TYPE;
		TurnLeftThrustEvent();
	};
	class TurnRightThrustEvent final : public Event {
	public:
		static const EventType TYPE;
		TurnRightThrustEvent();
	};

	class MainThrustEventEmitter final : public EventEmitter {
	public: virtual void emit(std::queue<Event::Ptr>& events) override;
	};
	class TurnLeftThrustEventEmitter final : public EventEmitter {
	public: virtual void emit(std::queue<Event::Ptr>& events) override;
	};
	class TurnRightThrustEventEmitter final : public EventEmitter {
	public: virtual void emit(std::queue<Event::Ptr>& events) override;
	};

	void mainThrust();
	void turnLeftThrust();
	void turnRightThrust();

	// Attack

	class FireEvent final : public Event {
	public:
		static const EventType TYPE;
		FireEvent();
	};

	class FireEventEmitter final : public EventEmitter {
	public: virtual void emit(std::queue<Event::Ptr>& events) override;
	};

	void fire();

	// Upgrades

	// Use the generic UpgradeEvent type / UpgradeEventEmitter for PlayerShip upgrades.

	void purchaseUpgrade(const Upgrade& upgrade);
};

/* Player Ship
-------------------- */

class PlayerShip final :
	public Ship,
	public HasEventHandlerOf<PlayerShipEventHandler>,
	public HasUpgradeTree,
	public HasUIOf<UpgradeTreeUI>
{
public:
	using Ptr = std::shared_ptr<PlayerShip>;

	// Lifecycle

	PlayerShip(PlayerShipSpec::Ptr spec);
	static const ObjectFactory factory;

	virtual void buildUpgradeTree(UpgradeTree& upgradeTree) override;

	// Delegate event factory dependency to components that need it.
	virtual void setObjectEventFactory(ObjectEventFactory::Ptr objectEventFactory) override;
};
