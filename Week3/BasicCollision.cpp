#include "BasicCollision.h"

#include "HasEventHandler.h"
#include "HasCollisionOf.h"
#include "BasicCollisionModel.h"

/* Basic Collision
-------------------------------------------------- */

BasicCollision::BasicCollision(ObjectManager::Ptr objectManager)
	: objectManager(objectManager) {
}

BasicCollision::UPtr BasicCollision::create(ObjectManager::Ptr objectManager) {
	return BasicCollision::UPtr(new BasicCollision(objectManager));
}

void BasicCollision::emit(std::queue<Event::Ptr>& events) {
	// It would be more efficient to track this incrementally, but we'd need object events to be broadcast to
	// more than just the object manager for that to be possible. This is an adequate solution for now.
	auto allGameObjects = objectManager->getAllGameObjects();
	std::list<HasCollisionOf<BasicCollisionModel>::Ptr> collidableObjects;
	for (GameObject::Ptr gameObject : allGameObjects) {
		if (auto collidableObject = std::dynamic_pointer_cast<HasCollisionOf<BasicCollisionModel>>(gameObject)) {
			collidableObjects.push_back(collidableObject);
		}
	}

	// Objects here cannot be nullptr (as they are deleted immediately) and cannot be deleted mid-collision
	// detection (as deletion is handled through events, which are not dispatched to the object manager until
	// the next phase of event processing this has finished).
	for (auto i = collidableObjects.begin(); i != collidableObjects.end(); i++) {
		for (auto j = std::next(i); j != collidableObjects.end(); j++) {
			if ((*i)->collisionModel().getShape().intersects((*j)->collisionModel().getShape())) {
				events.push(
					TargettedEvent::Ptr(new TargettedEvent(
						CollisionEvent::create(std::dynamic_pointer_cast<GameObject>(*i)),
						std::dynamic_pointer_cast<HasEventHandler>(*j)
					))
				);
				events.push(
					TargettedEvent::Ptr(new TargettedEvent(
						CollisionEvent::create(std::dynamic_pointer_cast<GameObject>(*j)),
						std::dynamic_pointer_cast<HasEventHandler>(*i)
					))
				);
			}
		}
	}
}

/* Collision Event
-------------------------------------------------- */

const EventType CollisionEvent::COLLISION;

CollisionEvent::CollisionEvent(GameObject::Ptr other) : Event(COLLISION), other(other) {}

CollisionEvent::Ptr CollisionEvent::create(GameObject::Ptr other) {
	return CollisionEvent::Ptr(new CollisionEvent(other));
}
