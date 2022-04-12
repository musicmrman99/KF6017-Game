#include "BasicCollisionModel.h"

BasicCollisionModel::BasicCollisionModel(
	ShapeUPtr shape,
	const BasicCollisionType& type,
	BasicCollisionTypeVector acceptedTypes
) : shape(move(shape)), type(type), acceptedTypes(acceptedTypes) {}

BasicCollisionModel::Ptr BasicCollisionModel::create(
	ShapeUPtr shape,
	const BasicCollisionType& type,
	BasicCollisionTypeVector acceptedTypes
) {
	return Ptr(new BasicCollisionModel(move(shape), type, acceptedTypes));
}

BasicCollisionModel::Ptr BasicCollisionModel::create(
	IShape2D* shape,
	const BasicCollisionType& type,
	BasicCollisionTypeVector acceptedTypes
) {
	return BasicCollisionModel::create(ShapeUPtr(shape), type, acceptedTypes);
}

void BasicCollisionModel::updateCollision() {
	shape->emplace(physModel().pos(), physModel().rot());
}

const IShape2D& BasicCollisionModel::getShape() const { return *shape.get(); }
const BasicCollisionType& BasicCollisionModel::getType() const { return type; }
const BasicCollisionModel::BasicCollisionTypeVector& BasicCollisionModel::getAcceptedTypes() const { return acceptedTypes; }
