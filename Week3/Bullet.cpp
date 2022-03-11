#include "Bullet.h"

#include "QueueUtils.h"
#include "uptrcast.h"

#include "ObjectEvent.h"
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

Bullet::Bullet(BulletSpec::UPtr spec, NewtonianPhysModel::Ptr physModel)
    : GameObject(
        Timer::UPtr(new Timer(4.0)),
        physModel,
        ImageGraphicsModel::UPtr(new ImageGraphicsModel(physModel, spec->image)),
        NullGraphicsModel::UPtr(new NullGraphicsModel())
    ) {
}

Bullet::Bullet(BulletSpec::UPtr spec)
    : Bullet(move(spec),
        NewtonianPhysModel::UPtr(new NewtonianPhysModel(spec->pos, spec->rot * SPEED, spec->rot, 0.0f))
    ) {
}

const ObjectFactory::Factory Bullet::factory = [](ObjectSpec::UPtr spec) {
    return Bullet::UPtr(new Bullet(static_unique_pointer_cast<BulletSpec>(move(spec))));
};

void Bullet::handle(const Event::Ptr e) {
    if (std::dynamic_pointer_cast<TimerEvent>(e)) {
        globalEventBuffer.push(DestroyObjectEvent::create(this));
    }
}

void Bullet::emit(std::queue<Event::Ptr>& globalEvents) {
    shiftInto(globalEventBuffer, globalEvents);
}
