#pragma once

#include <vector>
#include <map>
#include <memory>

#include "MyDrawEngine.h"

#include "GameObject.h"
#include "Event.h"
#include "NewtonianPhysModel.h"
#include "ImageGraphicsModel.h"

#include "UpgradeTree.h"

#include "ObjectFactory.h"
#include "ShipSpec.h"

class Ship final : public GameObject {
public:
	// Get/Set the right types

	virtual NewtonianPhysModel& physModel() override;
	virtual void setPhysModel(PhysModel::Ptr physModel) override;

	// Movement

	class MainThrustEvent final : public Event {};
	class TurnLeftThrustEvent final : public Event {};
	class TurnRightThrustEvent final : public Event {};

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

	class FireEvent final : public Event {};
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

	// 2nd phase constructor
	Ship(ShipSpec::UPtr spec, NewtonianPhysModel::Ptr physModel);

public:
	const UpgradeTree& getUpgradeTree();

	// Lifecycle

	Ship(ShipSpec::UPtr spec);
	static const ObjectFactory::Factory factory;
	void buildUpgradeTree();
	virtual ~Ship();

	virtual void beforeFrame() override;
	virtual void handle(const Event::Ptr e) override;
};
