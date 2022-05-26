#include "Integrity.h"

//#include "Explosion.h"

/* Initialisation
---------------------- */

Integrity::Integrity(
    PictureIndex explosionImage,
    float maxIntegrity
) :
    explosionImage(explosionImage),
    maxIntegrity(maxIntegrity),
    _integrity(maxIntegrity)
{}

Integrity::UPtr Integrity::create(PictureIndex explosionImage, float maxIntegrity) {
    return UPtr(new Integrity(explosionImage, maxIntegrity));
}

/* Shift Integrity Event
---------------------- */

// Event

const EventType Integrity::ShiftIntegrityEvent::TYPE;
Integrity::ShiftIntegrityEvent::ShiftIntegrityEvent(const float amount) :
    Event(TYPE),
    amount(amount)
{}
Integrity::ShiftIntegrityEventEmitter::ShiftIntegrityEventEmitter(float amount) : amount(amount) {}
void Integrity::ShiftIntegrityEventEmitter::emit(std::queue<Event::Ptr>& events) {
    events.push(ShiftIntegrityEvent::Ptr(new ShiftIntegrityEvent(amount)));
}

// Action (and getter)

float Integrity::integrity() const { return _integrity; }
void Integrity::shiftIntegrity(float amount) {
    _integrity += _integrity;
}

/* Event Handling
---------------------- */

void Integrity::handle(const Event::Ptr e) {
    if (e->type == ShiftIntegrityEvent::TYPE) {
        shiftIntegrity(std::static_pointer_cast<ShiftIntegrityEvent>(e)->amount);
    }
}
