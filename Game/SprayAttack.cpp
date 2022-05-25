#include "SprayAttack.h"

#include "realmod.h"

#include "BulletSpec.h"

/* Initialisation
---------------------- */

SprayAttack::SprayAttack(PictureIndex bulletImage) : bulletImage(bulletImage) {}
SprayAttack::UPtr SprayAttack::create(PictureIndex bulletImage) {
    return UPtr(new SprayAttack(bulletImage));
}

/* Rotate Firing Angle Event
---------------------- */

// Event

const EventType SprayAttack::RotateFiringAngleEvent::TYPE;
SprayAttack::RotateFiringAngleEvent::RotateFiringAngleEvent(const float amount) :
    Event(TYPE),
    amount(amount)
{}
SprayAttack::RotateFiringAngleEventEmitter::RotateFiringAngleEventEmitter(float amount) : amount(amount) {}
void SprayAttack::RotateFiringAngleEventEmitter::emit(std::queue<Event::Ptr>& events) {
    events.push(RotateFiringAngleEvent::Ptr(new RotateFiringAngleEvent(amount)));
}

// Action (and getter)

float SprayAttack::offsetAngle() const { return _offsetAngle; }
void SprayAttack::rotateOffsetAngle(float amount) {
    _offsetAngle = realmod(_offsetAngle + amount, 2.0f * (float) M_PI);
}

/* Fire Event
---------------------- */

// Event

const EventType SprayAttack::FireEvent::TYPE;
SprayAttack::FireEvent::FireEvent() : Event(TYPE) {}
void SprayAttack::FireEventEmitter::emit(std::queue<Event::Ptr>& events) {
    events.push(FireEvent::Ptr(new FireEvent()));
}

// Action

void SprayAttack::fire() {
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

/* Event Handling
---------------------- */

void SprayAttack::handle(const Event::Ptr e) {
    if (e->type == FireEvent::TYPE) fire();
}
