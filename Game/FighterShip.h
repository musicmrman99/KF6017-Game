#pragma once

// Dependencies
#include <memory>
#include "MyDrawEngine.h"

// Traits
#include "Ship.h"
#include "HasEventHandlerOf.h"
#include "HasComponent.h"

// Models
#include "MultiEventHandler.h"
#include "BasicMovement.h"
#include "SprayAttack.h"
#include "Integrity.h"

// Creation
#include "ObjectFactory.h"
#include "FighterShipSpec.h"

class FighterShip final :
	public Ship,
	public HasEventHandlerOf<MultiEventHandler>,
	public HasComponent<BasicMovement>,
	public HasComponent<SprayAttack>,
	public HasComponent<Integrity>
{
public:
	using Ptr = std::shared_ptr<FighterShip>;

	// Lifecycle

	FighterShip(FighterShipSpec::Ptr spec);
	static const ObjectFactory factory;

	// Delegate event factory dependency to components that need it.
	virtual void setObjectEventFactory(ObjectEventFactory::Ptr objectEventFactory) override;
};
