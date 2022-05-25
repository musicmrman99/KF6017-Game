#pragma once

// Dependencies
#include <memory>
#include "MyDrawEngine.h"

// Traits
#include "GameObject.h"
#include "HasEventEmitterOf.h"
#include "HasCollisionOf.h"
#include "HasPhysOf.h"
#include "HasGraphicsOf.h"
#include "ObjectEventCreator.h"

// Models
#include "BufferedEventEmitter.h"
#include "BasicCollisionModel.h"
#include "NewtonianPhysModel.h"
#include "ImageGraphicsModel.h"

// Creation
#include "ShipSpec.h"

class Ship :
	public GameObject,
	public HasEventEmitterOf<BufferedEventEmitter>,
	public HasCollisionOf<BasicCollisionModel>,
	public HasPhysOf<NewtonianPhysModel>,
	public HasGraphicsOf<ImageGraphicsModel>,
	public ObjectEventCreator
{
public:
	using Ptr = std::shared_ptr<Ship>;

	// Lifecycle

	Ship(ShipSpec::Ptr spec);

	virtual void beforeFrame() override;
};
