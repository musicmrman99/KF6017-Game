#include "FighterShipSpec.h"

const Vector2D FighterShipSpec::COLLISION_SIZE(100, 50);
const ObjectType FighterShipSpec::FIGHTER_SHIP;

FighterShipSpec::FighterShipSpec(
	const Vector2D pos,
	const Vector2D rot,
	const PictureIndex image,
	const PictureIndex bulletImage
) : ShipSpec(
		FIGHTER_SHIP,
		pos, rot,
		COLLISION_SIZE,
		image
	),
	bulletImage(bulletImage)
{}
