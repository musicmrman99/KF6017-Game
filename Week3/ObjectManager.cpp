#include "ObjectManager.h"

#include "ObjectEvent.h"

/* Object Manager
-------------------------------------------------- */

void ObjectManager::addObject(GameObject::UPtr gameObject) {
    objects.push_back(move(gameObject));
}

void ObjectManager::deleteObject(GameObject* gameObject) {
    objects.remove_if(
        [gameObject](const GameObject::Ptr& object) {
            return object.get() == gameObject;
        }
    );
}

void ObjectManager::run() {
    // Handle actions
    for (GameObject::Ptr& object : objects) object->beforeActions();
    for (GameObject::Ptr& object : objects) object->actions();

    // Handle Global Events
    for (GameObject::Ptr& object : objects) object->emit(events);
    while (!events.empty()) {
        // Handle Event
        const Event::Ptr& event = events.front();
        if (const TargettedEvent::Ptr targettedEvent = std::dynamic_pointer_cast<TargettedEvent>(event)) {
            if (auto ptr = targettedEvent->target.lock()) ptr->handle(move(targettedEvent->event));
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

    // Anything else
    for (GameObject::Ptr& object : objects) object->afterFrame();
}

void ObjectManager::handle(const Event::Ptr e) {
    if (auto roe = std::dynamic_pointer_cast<ReleaseObjectEvent>(e)) {
        addObject(move(roe->object));
    }
    else if (auto roe = std::dynamic_pointer_cast<DestroyObjectEvent>(e)) {
        deleteObject(roe->object);
    }
}

/* Events
-------------------------------------------------- */

// Release Object Event
ReleaseObjectEvent::ReleaseObjectEvent(GameObject::UPtr object)
    : Event(), object(move(object)) {
}

TargettedEvent::UPtr ReleaseObjectEvent::create(ObjectManager::WPtr objectManager, GameObject::UPtr object) {
    return TargettedEvent::UPtr(new TargettedEvent(
        ReleaseObjectEvent::UPtr(new ReleaseObjectEvent(
            move(object)
        )),
        std::static_pointer_cast<EventHandler>(objectManager.lock())
    ));
}

// Destroy Object Event
DestroyObjectEvent::DestroyObjectEvent(GameObject* object)
    : Event(), object(object) {
}

TargettedEvent::UPtr DestroyObjectEvent::create(ObjectManager::WPtr objectManager, GameObject* object) {
    return TargettedEvent::UPtr(new TargettedEvent(
        DestroyObjectEvent::UPtr(new DestroyObjectEvent(
            object
        )),
        std::static_pointer_cast<EventHandler>(objectManager.lock())
    ));
}
