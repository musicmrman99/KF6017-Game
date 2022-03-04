#include "Bullet.h"

/* Get/Set the right types
-------------------------------------------------- */

NewtonianPhysModel& Bullet::physModel() {
    return static_cast<NewtonianPhysModel&>(GameObject::physModel());
}
void Bullet::setPhysModel(PhysModelPtr physModel) {
    if (physModel && dynamic_cast<NewtonianPhysModel*>(physModel.get())) {
        GameObject::setPhysModel(physModel);
    }
}

/* Lifecycle
-------------------------------------------------- */

Bullet::Bullet(
    Vector2D pos, Vector2D rot, PictureIndex image,
    std::shared_ptr<NewtonianPhysModel> physModel
) : GameObject(
    std::shared_ptr<NullEventEmitter>(new NullEventEmitter()),
    physModel,
    std::shared_ptr<ImageGraphicsModel>(new ImageGraphicsModel(physModel, image)),
    std::shared_ptr<NullGraphicsModel>(new NullGraphicsModel())
) {}

Bullet::Bullet(Vector2D pos, Vector2D rot, PictureIndex image)
    : Bullet(
        pos, rot, image,
        std::shared_ptr<NewtonianPhysModel>(new NewtonianPhysModel(pos, rot * SPEED, rot, 0.0f))
    ) {
}

Bullet::~Bullet() {}
