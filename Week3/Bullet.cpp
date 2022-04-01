#include "Bullet.h"

#include "ptrcast.h"

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
        new AngledRectangle2D(Vector2D(0, 0), 20, 20),
        BulletSpec::BULLET_COLLISION,
        {} // Collides with everything (that has a collision model)
    )),
    HasPhysOf(NewtonianPhysModel::UPtr(new NewtonianPhysModel(spec->pos, spec->rot * SPEED, spec->rot, 0.0f))),
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
    timer = Timer::create(OBJECT_CULL_TIME, ref().lock()->eventHandlerWPtr());
    eventHandler().setTimer(timer);                           // DEPENDS: Timer
    eventEmitter().enqueue(
        objectEventFactory()->createObject(
            ControllerSpec::create(timer)
        )
    );
}

void Bullet::beforeDraw() {
    AngledRectangle2DSides sides = static_cast<const AngledRectangle2D&>(collisionModel().getShape()).GetSides();
    MyDrawEngine::GetInstance()->DrawLine(sides.top.GetStart(), sides.top.GetEnd(), MyDrawEngine::LIGHTRED);
    MyDrawEngine::GetInstance()->DrawLine(sides.left.GetStart(), sides.left.GetEnd(), MyDrawEngine::LIGHTRED);
    MyDrawEngine::GetInstance()->DrawLine(sides.bottom.GetStart(), sides.bottom.GetEnd(), MyDrawEngine::LIGHTRED);
    MyDrawEngine::GetInstance()->DrawLine(sides.right.GetStart(), sides.right.GetEnd(), MyDrawEngine::LIGHTRED);
}
