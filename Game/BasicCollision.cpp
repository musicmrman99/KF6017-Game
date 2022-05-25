#include "BasicCollision.h"

// Dependencies
#include <algorithm>
#include "ReferenceWrapperUtils.h"
#include "TargettedEvent.h"
#include "CollisionEvent.h"

// Traits
#include "HasCollisionOf.h"
#include "HasEventHandler.h"

// Models
#include "BasicCollisionModel.h"

BasicCollision::BasicCollision(BasicCollisionSpec::UPtr spec)
	: HasEventEmitterOf(BufferedEventEmitter::UPtr(new BufferedEventEmitter())) {
}

const ObjectFactory BasicCollision::factory = [](ObjectSpec::UPtr spec) {
	return GameObject::Ptr(new BasicCollision(static_unique_pointer_cast<BasicCollisionSpec>(move(spec))));
};

// Track collidable objects

void BasicCollision::objectCreated(GameObject::Ptr object) {
	if (auto collidableObject = std::dynamic_pointer_cast<HasCollisionOf<BasicCollisionModel>>(object)) {
		collidableObjects.push_back(collidableObject);
	}
}
void BasicCollision::objectDestroyed(GameObject::Ptr object) {
	if (auto collidableObject = std::dynamic_pointer_cast<HasCollisionOf<BasicCollisionModel>>(object)) {
		collidableObjects.remove(collidableObject);
	}
}

// Update the collision models and check for collisions

void BasicCollision::run() {
	for (const HasCollisionOf<BasicCollisionModel>::Ptr& collidableObject : collidableObjects) {
		collidableObject->updateCollision();
	}

	// Objects here cannot be nullptr, as object deletion is handled during the Events lifecycle point,
	// and objects are deleted immediately upon receiving the DestroyObjectEvent in that lifecycle point.
	for (auto i = collidableObjects.begin(); i != collidableObjects.end(); i++) {
		for (auto j = std::next(i); j != collidableObjects.end(); j++) {
			if ((*i)->collisionModel().getShape().intersects((*j)->collisionModel().getShape())) {
				// Tell J it has collided with I
				const auto& iType = (*i)->collisionModel().getType();
				const auto& jAcceptedTypes = (*j)->collisionModel().getAcceptedTypes();
				if (
					jAcceptedTypes.size() == 0 || // If the size is zero, accept anything
					std::find_if(
						jAcceptedTypes.begin(),
						jAcceptedTypes.end(),
						ReferenceWrapperEqPredicate<const BasicCollisionType>(iType)
					) != jAcceptedTypes.end()
				) {
					eventEmitter().enqueue(
						TargettedEvent::Ptr(new TargettedEvent(
							CollisionEvent::create(std::dynamic_pointer_cast<GameObject>(*i)),
							std::dynamic_pointer_cast<HasEventHandler>(*j)
						))
					);
				}

				// Tell I it has collided with J
				const auto& jType = (*j)->collisionModel().getType();
				const auto& iAcceptedTypes = (*i)->collisionModel().getAcceptedTypes();
				if (
					iAcceptedTypes.size() == 0 || // If the size is zero, accept anything
					std::find_if(
						iAcceptedTypes.begin(),
						iAcceptedTypes.end(),
						ReferenceWrapperEqPredicate<const BasicCollisionType>(jType)
					) != iAcceptedTypes.end()
				) {
					eventEmitter().enqueue(
						TargettedEvent::Ptr(new TargettedEvent(
							CollisionEvent::create(std::dynamic_pointer_cast<GameObject>(*j)),
							std::dynamic_pointer_cast<HasEventHandler>(*i)
						))
					);
				}
			}
		}
	}
}
