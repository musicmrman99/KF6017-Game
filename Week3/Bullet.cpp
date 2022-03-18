#include "Bullet.h"

#include "ptrcast.h"

/* Lifecycle
-------------------------------------------------- */

Bullet::Bullet(BulletSpec::UPtr spec) :
    HasPhysOf(NewtonianPhysModel::UPtr(new NewtonianPhysModel(spec->pos, spec->rot * SPEED, spec->rot, 0.0f))),
    HasGraphicsOf(ImageGraphicsModel::UPtr(new ImageGraphicsModel(spec->image))),
    timer(nullptr)
{
    trackPhysObserver(graphicsModelWPtr());
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
