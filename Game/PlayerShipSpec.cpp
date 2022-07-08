#include "PlayerShipSpec.h"

const Vector2D PlayerShipSpec::COLLISION_SIZE(100, 50);
const ObjectType PlayerShipSpec::PLAYER_SHIP;

PlayerShipSpec::PlayerShipSpec(
	const Vector2D pos,
	const Vector2D rot,
	const PictureIndex image,
	const float maxIntegrity,
	const PictureIndex bulletImage,
	const float bulletDamage
) : ShipSpec(
		PLAYER_SHIP,
		pos, rot,
		COLLISION_SIZE,
		image
	),
	maxIntegrity(maxIntegrity),
	bulletImage(bulletImage),
	bulletDamage(bulletDamage)
{}
