#pragma once

// Dependencies
#include <memory>
#include "MyDrawEngine.h"

// Traits
#include "Ship.h"
#include "HasEventHandlerOf.h"
#include "HasComponent.h"
#include "HasUpgradeTree.h" // Has a fixed model (UpgradeTree)
#include "HasUIOf.h"

// Models
#include "MultiEventHandler.h"
#include "BasicMovement.h"
#include "SprayAttack.h"
#include "Integrity.h"
#include "UpgradeTreeUI.h"

// Creation
#include "ObjectFactory.h"
#include "PlayerShipSpec.h"

// Define what upgrades are available.
struct PlayerShipUpgrade {
	static const Upgrade SHIP; // Root Upgrade

	static const Upgrade HEAVY_SHELLS;
	static const Upgrade PLASMA_CANNON;
	static const Upgrade ARMOURED_HULL;
	static const Upgrade SHIELDING;
};

// The player's ship.
class PlayerShip final :
	public Ship,
	public HasEventHandlerOf<MultiEventHandler>,
	public HasComponent<BasicMovement>,
	public HasComponent<SprayAttack>,
	public HasComponent<Integrity>,
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

	virtual void afterFrame() override;
};
