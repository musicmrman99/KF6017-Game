#include "ShipSpec.h"

const BasicCollisionType ShipSpec::SHIP_COLLISION;

ShipSpec::ShipSpec(
	const ObjectType& type,
	const Vector2D pos,
	const Vector2D rot,
	const Vector2D& collisionSize,
	const PictureIndex image
) :
	ObjectSpec(type),
	pos(pos),
	rot(rot),
	collisionSize(collisionSize),
	image(image)
{}
