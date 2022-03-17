#include "ObjectManager.h"

#include <algorithm>

#include "ObjectEvent.h"

/* Creation
-------------------------------------------------- */

ObjectManager::ObjectManager() : objectEventFactory(nullptr) {}
void ObjectManager::setSelf(WPtr me) {
    SelfReferencing<ObjectManager>::setSelf(me);
    // Not a nice solution (setSelf() isn't supposed to be responsible for this much initialisation), but it works (for now)
    objectEventFactory = ObjectEventFactory::create(self());
}

ObjectManager::Ptr ObjectManager::create() {
    Ptr ptr = Ptr(new ObjectManager());
    ptr->setSelf(ptr);
    return ptr;
}

/* Getters
-------------------------------------------------- */

ObjectFactoryManager& ObjectManager::getObjectFactoryManager() {
    return factory;
}

/* Event Handling
-------------------------------------------------- */

GameObject::Ptr ObjectManager::createObject(ObjectSpec::UPtr spec) {
    GameObject::Ptr object = factory.create(move(spec));
    if (!object) return nullptr;
    object->setObjectEventFactory(objectEventFactory);
    object->afterCreate();
    object->emit(events); // Flush event buffer in case controllers or game object require initialised object.
    objects.push_back(object);
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
        (*delObject)->beforeDestroy();
        (*delObject)->emit(events); // Emit any remaining buffered events before it's destroyed.
    }

    objects.erase(toDelete, objects.end());
}

void ObjectManager::addController(EventEmitter::Ptr controller) {
    if (!controller) return;
    if (auto c = std::dynamic_pointer_cast<ObjectEventEmitter>(controller)) {
        c->setObjectEventFactory(objectEventFactory);
    }
    controllers.push_back(controller);
}

void ObjectManager::removeController(EventEmitter::WPtr controller) {
    controllers.remove_if(
        [controller](const EventEmitter::Ptr& myController) {
            return myController == controller.lock();
        }
    );
}

void ObjectManager::handle(const Event::Ptr e) {
         if (e->type == CreateObjectEvent::TYPE) createObject(move(std::static_pointer_cast<CreateObjectEvent>(e)->spec)); // Discard the returned object for now
    else if (e->type == DestroyObjectEvent::TYPE) destroyObject(std::static_pointer_cast<DestroyObjectEvent>(e)->object);
    else if (e->type == AddControllerEvent::TYPE) addController(std::static_pointer_cast<AddControllerEvent>(e)->controller);
    else if (e->type == RemoveControllerEvent::TYPE) removeController(std::static_pointer_cast<RemoveControllerEvent>(e)->controller);
}

/* Frame Process
-------------------------------------------------- */

void ObjectManager::run() {
    // Anything first
    for (GameObject::Ptr& object : objects) object->beforeFrame();

    // Emit Global Events - Controllers
    for (EventEmitter::Ptr& controller : controllers) controller->emit(events);

    // Handle Global Events
    for (GameObject::Ptr& object : objects) object->emit(events);
    while (!events.empty()) {
        // Handle Event
        const Event::Ptr& event = events.front();
        if (event->type == TargettedEvent::TYPE) {
            const TargettedEvent::Ptr te = std::static_pointer_cast<TargettedEvent>(event);
            if (auto ptr = te->target.lock()) ptr->handle(move(te->event));
        } else {
            for (GameObject::Ptr& object : objects) object->handle(event);
        }
        events.pop();

        // Any more events to handle?
        for (GameObject::Ptr& object : objects) object->emit(events);
    }

    // Handle physics and graphics
    for (GameObject::Ptr& object : objects) object->beforePhys();
    for (GameObject::Ptr& object : objects) object->phys();

    for (GameObject::Ptr& object : objects) object->beforeDraw();
    for (GameObject::Ptr& object : objects) object->draw();

    for (GameObject::Ptr& object : objects) object->beforeDrawUI();
    for (GameObject::Ptr& object : objects) object->drawUI();

    // Anything last
    for (GameObject::Ptr& object : objects) object->afterFrame();
}
