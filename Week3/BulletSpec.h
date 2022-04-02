#pragma once

#include "ObjectSpec.h"

#include "BasicCollisionModel.h"
#include "Vector2D.h"
#include "MyDrawEngine.h"

class BulletSpec final : public ObjectSpec {
public:
	using Ptr = std::shared_ptr<BulletSpec>;
	using UPtr = std::unique_ptr<BulletSpec>;

	static const ObjectType BULLET;
	static const BasicCollisionType BULLET_COLLISION;

	const Vector2D pos;
	const Vector2D rot;
	const Vector2D vel;
	const PictureIndex image;

	BulletSpec(
		const Vector2D pos,
		const Vector2D rot,
		const Vector2D vel,
		const PictureIndex image
	);
};
