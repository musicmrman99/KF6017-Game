#pragma once

#include <vector>
#include <map>
#include <memory>

#include "Node.h"

#include "MyDrawEngine.h"
#include "MyInputs.h"

#include "GameObject.h"
#include "Event.h"
#include "NewtonianPhysModel.h"
#include "ImageGraphicsModel.h"

#include "UpgradeTree.h"

class Ship : public GameObject {
public:
	// Get/Set the right types

	virtual NewtonianPhysModel& physModel() override;
	virtual void setPhysModel(PhysModelPtr physModel) override;

	// Movement

	static const EventTypePtr MAIN_THRUST;
	static const EventTypePtr TURN_LEFT_THRUST;
	static const EventTypePtr TURN_RIGHT_THRUST;

	void mainThrust();
	void turnLeftThrust();
	void turnRightThrust();

	// Upgrades

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

	// 2nd phase constructor
	Ship(
		Vector2D pos, Vector2D rot, PictureIndex image,
		std::shared_ptr<NewtonianPhysModel> physModel
	);

public:
	// Lifecycle
	Ship(Vector2D pos, Vector2D rot, PictureIndex image);
	void buildUpgradeTree();
	virtual ~Ship();

	virtual void beforeActions() override;
	virtual void handle(const Event& e) override;
};
