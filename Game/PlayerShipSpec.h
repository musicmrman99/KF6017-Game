#pragma once

#include "ObjectSpec.h"
#include "ShipSpec.h"

#include "BasicCollisionModel.h"
#include "Vector2D.h"
#include "MyDrawEngine.h"

class PlayerShipSpec final : public ShipSpec {
private:
	static const Vector2D COLLISION_SIZE;

public:
	using Ptr = std::shared_ptr<PlayerShipSpec>;
	using UPtr = std::unique_ptr<PlayerShipSpec>;

	static const ObjectType PLAYER_SHIP;

	const PictureIndex bulletImage;

	PlayerShipSpec(
		const Vector2D pos,
		const Vector2D rot,
		const PictureIndex image,
		const PictureIndex bulletImage
	);
};
