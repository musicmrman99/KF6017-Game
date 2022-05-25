#pragma once

#include "GameObject.h"
#include "Event.h"

class CollisionEvent final : public Event {
private:
	CollisionEvent(GameObject::Ptr other);

public:
	static const EventType TYPE;

	GameObject::Ptr other;

	static CollisionEvent::Ptr create(GameObject::Ptr other);
};
