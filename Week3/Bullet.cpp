#include "Bullet.h"

#include "uptrcast.h"

#include "ObjectEvent.h"

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
        physModel,
        ImageGraphicsModel::UPtr(new ImageGraphicsModel(physModel, spec->image)),
        NullGraphicsModel::UPtr(new NullGraphicsModel())
    ),
    timer(nullptr) {
}

const ObjectFactory::Factory Bullet::factory = [](ObjectSpec::UPtr spec) {
    BulletSpec::UPtr bulletSpec = static_unique_pointer_cast<BulletSpec>(move(spec));
    Bullet::Ptr bullet = Bullet::Ptr(new Bullet(
        move(bulletSpec),
        NewtonianPhysModel::UPtr(new NewtonianPhysModel(bulletSpec->pos, bulletSpec->rot * SPEED, bulletSpec->rot, 0.0f))
    ));
    bullet->setSelf(bullet);
    return bullet;
};

void Bullet::afterCreate() {
    timer = Timer::create(4.0, self());
    enqueue(AddControllerEvent::create(timer));
}

void Bullet::handle(const Event::Ptr e) {
    auto te = std::dynamic_pointer_cast<TimerEvent>(e);
    if (te && te->timer.lock() == timer) {
        enqueue(DestroyObjectEvent::create(this));
    }
}
