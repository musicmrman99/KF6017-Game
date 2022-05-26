#include "FighterShipSpec.h"

const Vector2D FighterShipSpec::COLLISION_SIZE(100, 50);
const ObjectType FighterShipSpec::FIGHTER_SHIP;

FighterShipSpec::FighterShipSpec(
	const Vector2D pos,
	const Vector2D rot,
	const PictureIndex image,
	const float maxIntegrity,
	const PictureIndex bulletImage,
	const float bulletDamage
) : ShipSpec(
		FIGHTER_SHIP,
		pos, rot,
		COLLISION_SIZE,
		image
	),
	maxIntegrity(maxIntegrity),
	bulletImage(bulletImage),
	bulletDamage(bulletDamage)
{}
