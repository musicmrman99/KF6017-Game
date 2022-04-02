#pragma once

#include <memory>

#include "EventEmitter.h"
#include "ObjectManager.h"

// Handles tracking collidable objects and emitting collision events.
class BasicCollision final : public EventEmitter {
private:
	ObjectManager::Ptr objectManager;
	BasicCollision(ObjectManager::Ptr objectManager);

public:
	using Ptr = std::shared_ptr<BasicCollision>;
	using UPtr = std::unique_ptr<BasicCollision>;
	using WPtr = std::weak_ptr<BasicCollision>;

	static BasicCollision::UPtr create(ObjectManager::Ptr objectManager);
	virtual void emit(std::queue<Event::Ptr>& events) override;
};

class CollisionEvent final : public Event {
private:
	CollisionEvent(GameObject::Ptr other);

public:
	static const EventType COLLISION;

	GameObject::Ptr other;

	static CollisionEvent::Ptr create(GameObject::Ptr other);
};
