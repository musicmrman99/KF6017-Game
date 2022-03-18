#include "Bullet.h"

#include "uptrcast.h"

/* Lifecycle
-------------------------------------------------- */

Bullet::Bullet(BulletSpec::UPtr spec)
    : GameObject(
        ImageGraphicsModel::UPtr(new ImageGraphicsModel(spec->image)),
        NullGraphicsModel::UPtr(new NullGraphicsModel())
    ),
    HasPhysOf(NewtonianPhysModel::UPtr(new NewtonianPhysModel(spec->pos, spec->rot * SPEED, spec->rot, 0.0f))),
    timer(nullptr) {
    // NOTE: The cast will disappear once we get the HasGraphics trait implemented
    trackPhysObserver(std::static_pointer_cast<ImageGraphicsModel>(_graphicsModel));
}

const ObjectFactory Bullet::factory = [](ObjectSpec::UPtr spec) {
    Bullet::Ptr bullet = Bullet::Ptr(new Bullet(static_unique_pointer_cast<BulletSpec>(move(spec))));
    bullet->setSelf(bullet);
    return bullet;
};

void Bullet::afterCreate() {
    timer = Timer::create(OBJECT_CULL_TIME, self());
    enqueue(objectEventFactory()->addController(timer));
}

void Bullet::handle(const Event::Ptr e) {
    if (
        e->type == TimerEvent::TYPE &&
        std::static_pointer_cast<TimerEvent>(e)->timer.lock() == timer
    ) {
        enqueue(objectEventFactory()->destroyObject(self()));
    }
}
