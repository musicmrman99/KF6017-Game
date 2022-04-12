#pragma once

#include "ObjectSpec.h"

#include "mydrawengine.h"

class StarFieldSpec final : public ObjectSpec {
private:
	StarFieldSpec(
		const Vector2D pos,
		const Vector2D size,
		const float density,
		const PictureIndex image
	);

public:
	using Ptr = std::shared_ptr<StarFieldSpec>;
	using UPtr = std::unique_ptr<StarFieldSpec>;

	static const ObjectType STAR_FIELD;

	const Vector2D pos;
	const Vector2D size;
	const float density;
	const PictureIndex image;

	static UPtr create(
		const Vector2D pos,
		const Vector2D size,
		const float density,
		const PictureIndex image
	);
};
