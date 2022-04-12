#include "Physics.h"

Physics::Ptr Physics::create() {
    return Ptr(new Physics());
}

// Track collidable objects

void Physics::objectCreated(GameObject::Ptr object) {
    if (auto physObject = std::dynamic_pointer_cast<HasPhys>(object)) {
        physObjects.push_back(physObject);
    }
}
void Physics::objectDestroyed(GameObject::Ptr object) {
    if (auto physObject = std::dynamic_pointer_cast<HasPhys>(object)) {
        physObjects.remove(physObject);
    }
}

void Physics::run() {
    for (HasPhys::Ptr& object : physObjects) object->beforePhys();
    for (HasPhys::Ptr& object : physObjects) object->phys();
}
