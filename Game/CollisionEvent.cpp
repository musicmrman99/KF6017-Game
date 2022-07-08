#include "CollisionEvent.h"

const EventType CollisionEvent::TYPE;

CollisionEvent::CollisionEvent(GameObject::Ptr other) : Event(TYPE), other(other) {}

CollisionEvent::Ptr CollisionEvent::create(GameObject::Ptr other) {
	return CollisionEvent::Ptr(new CollisionEvent(other));
}
