#pragma once

#include <memory>

#include "MyDrawEngine.h"

#include "GameObject.h"
#include "HasEventEmitterOf.h"
#include "HasEventHandlerOf.h"
#include "HasPhysOf.h"
#include "HasGraphicsOf.h"
#include "HasUIOf.h"

#include "Event.h"
#include "ObjectEventCreator.h"
#include "BufferedEventEmitter.h"
#include "NewtonianPhysModel.h"
#include "ImageGraphicsModel.h"

#include "HasUpgradeTree.h"
#include "UpgradeTreeUI.h"

#include "ObjectFactory.h"
#include "ShipSpec.h"

/* Static Definitions
-------------------- */

// Define what upgrades are available.
namespace ShipUpgrade {
	const Upgrade SHIP(L"Ship"); // Root Upgrade

	const Upgrade LOAD_OPTIMISATION(L"Load Optimisation");
	const Upgrade SPACIAL_COMPRESSION(L"Spacial Compression");
	const Upgrade COOPERATION(L"Cooperation");
	const Upgrade OPTIMAL_SELECTION(L"Optimal Selection");

	const Upgrade FRONT_THRUSTERS(L"Front Thrusters");
	const Upgrade REAR_THRUSTERS(L"Rear Thrusters");
	const Upgrade OVERDRIVE(L"Overdrive");
	const Upgrade HYPER_JUMP(L"Hyper Jump");

	const Upgrade HEAVY_SHELLS(L"Heavy Shells");
	const Upgrade FRONT_AUTO_CANNONS(L"Front Auto-Cannons");
	const Upgrade REAR_AUTO_CANNONS(L"Rear Auto-Cannons");
	const Upgrade IONIC_SHELLS(L"Ionic Shells");

	const Upgrade WORKER_DRONE(L"Worker Drone");
	const Upgrade ARMOURED_DRONE(L"Armoured Drone");
	const Upgrade MINE(L"Mine");
	const Upgrade FIGHTER_DRONE(L"Fighter Drone");
};

/* Event Handler
-------------------- */

class ShipEventHandler final :
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
	using UPtr = std::unique_ptr<ShipEventHandler>;

	ShipEventHandler(ShipSpec::Ptr spec);

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

	// Use the generic UpgradeEvent type / UpgradeEventEmitter for Ship upgrades.

	void purchaseUpgrade(const Upgrade& upgrade);
};

/* Ship
-------------------- */

class Ship final :
	public GameObject,
	public HasEventHandlerOf<ShipEventHandler>,
	public HasEventEmitterOf<BufferedEventEmitter>,
	public HasPhysOf<NewtonianPhysModel>,
	public HasGraphicsOf<ImageGraphicsModel>,
	public HasUpgradeTree,
	public HasUIOf<UpgradeTreeUI>,
	public ObjectEventCreator // FIXME: well, it's not really - but we need the reference, and ObjectManager will only give it to us if we are one of these.
{
public:
	// Lifecycle

	Ship(ShipSpec::Ptr spec);
	static const ObjectFactory factory;

	virtual ~Ship();

	virtual void buildUpgradeTree(UpgradeTree& upgradeTree) override;

	// FIXME: Dirty hack to make this work for now
	virtual void setObjectEventFactory(ObjectEventFactory::Ptr objectEventFactory) override;

	virtual void beforeFrame() override;
};
