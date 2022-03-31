#include "BasicCollisionModel.h"

BasicCollisionModel::BasicCollisionModel(ShapeUPtr shape) {
	this->shape = move(shape);
}

BasicCollisionModel::Ptr BasicCollisionModel::create(ShapeUPtr shape) {
	return Ptr(new BasicCollisionModel(move(shape)));
}

BasicCollisionModel::Ptr BasicCollisionModel::create(IShape2D* shape) {
	return BasicCollisionModel::create(ShapeUPtr(shape));
}

void BasicCollisionModel::updateCollision() {
	shape->emplace(physModel().pos(), physModel().rot());
}

IShape2D& BasicCollisionModel::getShape() {
	return *shape.get();
}
