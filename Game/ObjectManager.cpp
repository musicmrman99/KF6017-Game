#include "ObjectManager.h"

#include <algorithm>

#include "MyDrawEngine.h"

#include "TargettedEvent.h"
#include "ObjectEvent.h"
#include "ObjectEventCreator.h"

/* Creation
-------------------------------------------------- */

ObjectManager::ObjectManager() : objectEventFactory(nullptr) {}
void ObjectManager::setRef(WPtr me) {
    Referencing<ObjectManager>::setRef(me);
    // Not a nice solution (setRef() isn't supposed to be responsible for this much initialisation), but it works (for now)
    objectEventFactory = ObjectEventFactory::create(ref());
}

ObjectManager::Ptr ObjectManager::create() {
    Ptr ptr = Ptr(new ObjectManager());
    ptr->setRef(ptr);
    return ptr;
}

/* Getters
-------------------------------------------------- */

ObjectFactoryManager& ObjectManager::getObjectFactoryManager() { return factory; }
std::list<GameObject::Ptr>& ObjectManager::getAllGameObjects() { return objects; }

/* Event Handling
-------------------------------------------------- */

void ObjectManager::addLifecyclePoint(LifecyclePoint::Ptr lifecyclePoint) {
    lifecyclePoints.push_back(lifecyclePoint);
}

GameObject::Ptr ObjectManager::createObject(ObjectSpec::UPtr spec) {
    // Create and add to main list
    GameObject::Ptr object = factory.create(move(spec));
    if (!object) return nullptr;
    objects.push_back(object);

    // Is object event creator?
    if (auto c = std::dynamic_pointer_cast<ObjectEventCreator>(object)) {
        c->setObjectEventFactory(objectEventFactory);
    }

    // Run creation lifecycle method
    object->afterCreate();

    // Inform all lifecycle points
    for (auto& lifecyclePoint : lifecyclePoints) lifecyclePoint->objectCreated(object);

    // Return it
    return object;
}

void ObjectManager::destroyObject(GameObject::WPtr object) {
    std::list<GameObject::Ptr>::iterator toDelete = std::partition(
        objects.begin(), objects.end(),
        [object](const GameObject::Ptr& myObject) {
            return myObject != object.lock();
        }
    );

    // We're not expecting there to be multiple matches, but just in case.
    for (auto delObject = toDelete; delObject != objects.end(); ++delObject) {
        // Run destruction lifecycle and inform all lifecycle points
        (*delObject)->beforeDestroy();

        // Inform all lifecycle points
        for (auto& lifecyclePoint : lifecyclePoints) lifecyclePoint->objectDestroyed(*delObject);
    }

    // Remove from main list
    objects.erase(toDelete, objects.end());
}

void ObjectManager::handle(const Event::Ptr e) {
         if (e->type == CreateObjectEvent::TYPE) createObject(move(std::static_pointer_cast<CreateObjectEvent>(e)->spec)); // Discard the returned object for now
    else if (e->type == DestroyObjectEvent::TYPE) destroyObject(std::static_pointer_cast<DestroyObjectEvent>(e)->object);
}

/* Frame Process
-------------------------------------------------- */

void ObjectManager::run() {
    // Anything first
    for (GameObject::Ptr& object : objects) object->beforeFrame();

    // Run all lifecycle points
    for (LifecyclePoint::Ptr& lifecyclePoint : lifecyclePoints) lifecyclePoint->run();

    // Anything last
    for (GameObject::Ptr& object : objects) object->afterFrame();
}
