#include "Bullet.h"

/* Get/Set the right types
-------------------------------------------------- */

NewtonianPhysModel& Bullet::physModel() {
    return static_cast<NewtonianPhysModel&>(GameObject::physModel());
}
// Can't enforce uniqueness - GameObject interface requires shared_ptr.
void Bullet::setPhysModel(PhysModel::Ptr physModel) {
    if (physModel) {
        if (auto newtonianPhysModel = std::dynamic_pointer_cast<NewtonianPhysModel>(physModel)) {
            GameObject::setPhysModel(physModel);
            static_cast<ImageGraphicsModel&>(GameObject::graphicsModel()).setPhysModel(newtonianPhysModel);
        }
    }
}

/* Lifecycle
-------------------------------------------------- */

Bullet::Bullet(
    Vector2D pos, Vector2D rot, PictureIndex image,
    NewtonianPhysModel::Ptr physModel
) : GameObject(
    std::shared_ptr<NullEventEmitter>(new NullEventEmitter()),
    physModel,
    std::shared_ptr<ImageGraphicsModel>(new ImageGraphicsModel(physModel, image)),
    std::shared_ptr<NullGraphicsModel>(new NullGraphicsModel())
) {}

Bullet::Bullet(Vector2D pos, Vector2D rot, PictureIndex image)
    : Bullet(
        pos, rot, image,
        NewtonianPhysModel::UPtr(new NewtonianPhysModel(pos, rot * SPEED, rot, 0.0f))
    ) {
}

Bullet::~Bullet() {}
