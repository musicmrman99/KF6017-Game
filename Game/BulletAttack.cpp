#include "BulletAttack.h"

#include "BulletSpec.h"

BulletAttack::BulletAttack(PictureIndex bulletImage) : bulletImage(bulletImage) {}
BulletAttack::UPtr BulletAttack::create(PictureIndex bulletImage) {
    return UPtr(new BulletAttack(bulletImage));
}

// Event

const EventType BulletAttack::FireEvent::TYPE;
BulletAttack::FireEvent::FireEvent() : Event(TYPE) {}
void BulletAttack::FireEventEmitter::emit(std::queue<Event::Ptr>& events) {
    events.push(FireEvent::Ptr(new FireEvent()));
}

// Action

void BulletAttack::fire() {
    static constexpr float DIST = 40.0f;
    eventEmitter().enqueue(objectEventFactory()->createObject(
        BulletSpec::UPtr(new BulletSpec(
            physModel().pos() + DIST * physModel().rot(),
            physModel().rot(),
            physModel().vel(),
            bulletImage
        ))
    ));
}

void BulletAttack::handle(const Event::Ptr e) {
    if (e->type == FireEvent::TYPE) fire();
}
