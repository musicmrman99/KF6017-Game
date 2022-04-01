#include "BulletSpec.h"

const ObjectType BulletSpec::BULLET;
const BasicCollisionType BulletSpec::BULLET_COLLISION;

BulletSpec::BulletSpec(
	const Vector2D pos,
	const Vector2D rot,
	const PictureIndex image
) : ObjectSpec(BULLET), pos(pos), rot(rot), image(image) {}
