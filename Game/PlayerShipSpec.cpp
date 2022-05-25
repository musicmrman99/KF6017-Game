#include "PlayerShipSpec.h"

const Vector2D PlayerShipSpec::COLLISION_SIZE(100, 50);
const ObjectType PlayerShipSpec::PLAYER_SHIP;

PlayerShipSpec::PlayerShipSpec(
	const Vector2D pos,
	const Vector2D rot,
	const PictureIndex image,
	const PictureIndex bulletImage
) : ShipSpec(
		PLAYER_SHIP,
		pos, rot,
		COLLISION_SIZE,
		image
	),
	bulletImage(bulletImage)
{}
