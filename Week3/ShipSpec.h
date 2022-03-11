#pragma once

#include "ObjectSpec.h"

#include "Vector2D.h"
#include "MyDrawEngine.h"

class ShipSpec final : public ObjectSpec {
public:
	using UPtr = std::unique_ptr<ShipSpec>;

	static const ObjectType SHIP;

	const Vector2D pos;
	const Vector2D rot;
	const PictureIndex image;
	const PictureIndex bulletImage;

	ShipSpec(
		const Vector2D pos,
		const Vector2D rot,
		const PictureIndex image,
		const PictureIndex bulletImage
	);
};
