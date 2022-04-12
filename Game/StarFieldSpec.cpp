#include "StarFieldSpec.h"

const ObjectType StarFieldSpec::STAR_FIELD;

StarFieldSpec::StarFieldSpec(
	const Vector2D pos,
	const Vector2D size,
	const float density,
	const PictureIndex image
)
	: ObjectSpec(STAR_FIELD),
	pos(pos),
	size(size),
	density(density),
	image(image) {
}

StarFieldSpec::UPtr StarFieldSpec::create(
	const Vector2D pos,
	const Vector2D size,
	const float density,
	const PictureIndex image
) {
	return UPtr(new StarFieldSpec(pos, size, density, image));
}
