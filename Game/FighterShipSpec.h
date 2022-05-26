#pragma once

#include "ObjectSpec.h"
#include "ShipSpec.h"

#include "BasicCollisionModel.h"
#include "Vector2D.h"
#include "MyDrawEngine.h"

class FighterShipSpec final : public ShipSpec {
private:
	static const Vector2D COLLISION_SIZE;

public:
	using Ptr = std::shared_ptr<FighterShipSpec>;
	using UPtr = std::unique_ptr<FighterShipSpec>;

	static const ObjectType FIGHTER_SHIP;

	const float maxIntegrity;
	const PictureIndex bulletImage;
	const float bulletDamage;

	FighterShipSpec(
		const Vector2D pos,
		const Vector2D rot,
		const PictureIndex image,
		const float maxIntegrity,
		const PictureIndex bulletImage,
		const float bulletDamage
	);
};
