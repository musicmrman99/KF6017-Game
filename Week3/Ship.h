#pragma once

#include <vector>
#include <map>
#include <memory>

#include "MyDrawEngine.h"

#include "GameObject.h"
#include "HasPhysOf.h"
#include "HasGraphicsOf.h"
#include "HasUIOf.h"

#include "Event.h"
#include "NewtonianPhysModel.h"
#include "ImageGraphicsModel.h"

#include "UpgradeTree.h"
#include "UpgradeTreeUI.h"

#include "ObjectFactory.h"
#include "ShipSpec.h"

class Ship final :
	public GameObject,
	public HasPhysOf<NewtonianPhysModel>,
	public HasGraphicsOf<ImageGraphicsModel>,
	public HasUIOf<UpgradeTreeUI>
{
public:
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

	static const Upgrade SHIP; // Root Upgrade

	static const Upgrade LOAD_OPTIMISATION;
	static const Upgrade COOPERATION;
	static const Upgrade OPTIMAL_SELECTION;
	static const Upgrade SPACIAL_COMPRESSION;

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

private:
	// Gameplay
	float engineThrust;
	float rotateThrust;

	UpgradeTree upgradeTree;

	PictureIndex bulletImage;

public:
	const UpgradeTree& getUpgradeTree();

	// Lifecycle

	Ship(ShipSpec::UPtr spec);
	static const ObjectFactory factory;
	void buildUpgradeTree();

	virtual ~Ship();

	virtual void beforeFrame() override;
	virtual void handle(const Event::Ptr e) override;
};
