#include "ShipSpec.h"

const ObjectType ShipSpec::SHIP;

ShipSpec::ShipSpec(
	const Vector2D pos,
	const Vector2D rot,
	const PictureIndex image,
	const PictureIndex bulletImage
) : ObjectSpec(SHIP),
	pos(pos),
	rot(rot),
	image(image),
	bulletImage(bulletImage) {
}
