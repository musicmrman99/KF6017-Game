#include "Bullet.h"

#include "QueueUtils.h"
#include "Timer.h"

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
    Vector2D pos, Vector2D rot, PictureIndex image, ObjectManager::WPtr objectManager,
    NewtonianPhysModel::Ptr physModel
) : GameObject(
        Timer::UPtr(new Timer(4.0)),
        physModel,
        ImageGraphicsModel::UPtr(new ImageGraphicsModel(physModel, image)),
        NullGraphicsModel::UPtr(new NullGraphicsModel())
    ),
    objectManager(objectManager) {
}

Bullet::Bullet(Vector2D pos, Vector2D rot, PictureIndex image, ObjectManager::WPtr objectManager)
    : Bullet(
        pos, rot, image, objectManager,
        NewtonianPhysModel::UPtr(new NewtonianPhysModel(pos, rot * SPEED, rot, 0.0f))
    ) {
}

void Bullet::handle(const Event::Ptr e) {
    if (std::dynamic_pointer_cast<TimerEvent>(e)) {
        globalEventBuffer.push(DestroyObjectEvent::create(objectManager, this));
    }
}

void Bullet::emit(std::queue<Event::Ptr>& globalEvents) {
    shiftInto(globalEventBuffer, globalEvents);
}
