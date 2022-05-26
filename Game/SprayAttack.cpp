#include "SprayAttack.h"

#include "realmod.h"

#include "BulletSpec.h"

/* Initialisation
---------------------- */

SprayAttack::SprayAttack(
    PictureIndex bulletImage,
    float bulletDamage
) :
    bulletImage(bulletImage),
    bulletDamage(bulletDamage),
    _offsetAngle(0.0f)
{}
SprayAttack::UPtr SprayAttack::create(PictureIndex bulletImage, float bulletDamage) {
    return UPtr(new SprayAttack(bulletImage, bulletDamage));
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

    Vector2D rot;
    rot.setBearing(physModel().rot().angle() + _offsetAngle, 1);

    eventEmitter().enqueue(objectEventFactory()->createObject(
        BulletSpec::UPtr(new BulletSpec(
            physModel().pos() + DIST * rot,
            rot,
            physModel().vel(),
            bulletImage,
            bulletDamage
        ))
    ));
}

/* Event Handling
---------------------- */

void SprayAttack::handle(const Event::Ptr e) {
    if (e->type == FireEvent::TYPE) fire();
    else if (e->type == RotateFiringAngleEvent::TYPE) {
        rotateOffsetAngle(std::static_pointer_cast<RotateFiringAngleEvent>(e)->amount);
    }
}
