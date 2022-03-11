#pragma once

#include "ObjectSpec.h"

#include "Vector2D.h"
#include "MyDrawEngine.h"

class BulletSpec final : public ObjectSpec {
public:
	using UPtr = std::unique_ptr<BulletSpec>;

	static const ObjectType BULLET;

	const Vector2D pos;
	const Vector2D rot;
	const PictureIndex image;

	BulletSpec(
		const Vector2D pos,
		const Vector2D rot,
		const PictureIndex image
	);
};
