#include "Bullet.h"

#include "ptrcast.h"

#include "ShipSpec.h"

/* BulletEventHandler
-------------------------------------------------- */

BulletEventHandler::BulletEventHandler() : timer(nullptr) {}
void BulletEventHandler::setTimer(Timer::Ptr timer) { this->timer = timer; }

void BulletEventHandler::handle(const Event::Ptr e) {
    if (
        e->type == TimerEvent::TYPE &&
        std::static_pointer_cast<TimerEvent>(e)->timer.lock() == timer
    ) {
        eventEmitter().enqueue(objectEventFactory()->destroyObject(ref()));
    }
}

/* Bullet
-------------------------------------------------- */

Bullet::Bullet(BulletSpec::UPtr spec) :
    HasEventHandlerOf(BulletEventHandler::UPtr(new BulletEventHandler())),
    HasEventEmitterOf(BufferedEventEmitter::UPtr(new BufferedEventEmitter())),
    HasCollisionOf(BasicCollisionModel::create(
        new Circle2D(COLLISION_RADIUS),
        BulletSpec::BULLET_COLLISION,
        { ShipSpec::SHIP_COLLISION }
    )),
    HasPhysOf(NewtonianPhysModel::UPtr(new NewtonianPhysModel(
        spec->pos,
        spec->vel + spec->rot * BASE_SPEED,
        spec->rot,
        0.0f
    ))),
    HasGraphicsOf(ImageGraphicsModel::UPtr(new ImageGraphicsModel(spec->image))),
    timer(nullptr)
{
    trackPhysObserver(graphicsModelWPtr());
    trackPhysObserver(collisionModelWPtr());
    trackEventEmitterObserver(eventHandlerWPtr());            // DEPENDS: EventEmitter
}

const ObjectFactory Bullet::factory = [](ObjectSpec::UPtr spec) {
    Bullet::Ptr bullet = Bullet::Ptr(new Bullet(static_unique_pointer_cast<BulletSpec>(move(spec))));
    bullet->setRef(bullet);
    bullet->eventHandler().setRef(bullet);                    // DEPENDS: Bullet
    return bullet;
};

void Bullet::setObjectEventFactory(ObjectEventFactory::Ptr objectEventFactory) {
    ObjectEventCreator::setObjectEventFactory(objectEventFactory);
    eventHandler().setObjectEventFactory(objectEventFactory); // DEPENDS: ObjectEventFactory
}

void Bullet::afterCreate() {
    // Send the timer event back to the event handler component directly
    timer = Timer::create(LIFETIME, ref().lock()->eventHandlerWPtr());
    eventHandler().setTimer(timer);                           // DEPENDS: Timer
    eventEmitter().enqueue(
        objectEventFactory()->createObject(
            ControllerSpec::create(timer)
        )
    );
}