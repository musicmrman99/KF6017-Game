#include "ObjectManager.h"

#include <algorithm>

#include "ObjectFactory.h"
#include "ObjectEvent.h"

GameObject::Ptr ObjectManager::createObject(ObjectSpec::UPtr spec) {
    GameObject::Ptr object = ObjectFactory::create(move(spec));
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
    controllers.push_back(controller);
}

void ObjectManager::removeController(EventEmitter::WPtr controller) {
    controllers.remove_if(
        [controller](const EventEmitter::Ptr& myController) {
            return myController == controller.lock();
        }
    );
}

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
        if (const TargettedEvent::Ptr targettedEvent = std::dynamic_pointer_cast<TargettedEvent>(event)) {
            if (auto ptr = targettedEvent->target.lock()) ptr->handle(move(targettedEvent->event));
        } else {
            for (GameObject::Ptr& object : objects) object->handle(event);
            handle(event);
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

void ObjectManager::handle(const Event::Ptr e) {
         if (auto coe = std::dynamic_pointer_cast<CreateObjectEvent>(e)) createObject(move(coe->spec)); // Discard the returned object for now
    else if (auto roe = std::dynamic_pointer_cast<DestroyObjectEvent>(e)) destroyObject(roe->object);
    else if (auto ace = std::dynamic_pointer_cast<AddControllerEvent>(e)) addController(ace->controller);
    else if (auto rce = std::dynamic_pointer_cast<RemoveControllerEvent>(e)) removeController(rce->controller);
}
