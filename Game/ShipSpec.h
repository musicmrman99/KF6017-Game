#pragma once

#include "ObjectSpec.h"

#include "BasicCollisionModel.h"
#include "Vector2D.h"
#include "MyDrawEngine.h"

class ShipSpec : public ObjectSpec {
public:
	using Ptr = std::shared_ptr<ShipSpec>;
	using UPtr = std::unique_ptr<ShipSpec>;

	static const BasicCollisionType SHIP_COLLISION;

	// Unique to each ship
	const Vector2D pos;
	const Vector2D rot;

	// Unique to each ship type
	const Vector2D& collisionSize;
	const PictureIndex image;

	ShipSpec(
		const ObjectType& type,
		const Vector2D pos,
		const Vector2D rot,
		const Vector2D& collisionSize,
		const PictureIndex image
	);
};
