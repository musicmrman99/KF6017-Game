#include "ObjectManager.h"

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
            if (auto ptr = targettedEvent->target.lock()) ptr->handle(event);
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
    if (EventTypeManager::isOfType(e->type, ReleaseObjectEvent::EVENT_TYPE)) {
        addObject(move(std::static_pointer_cast<ReleaseObjectEvent>(e)->object));
    }
    else if (EventTypeManager::isOfType(e->type, DestroyObjectEvent::EVENT_TYPE)) {
        deleteObject(std::static_pointer_cast<const DestroyObjectEvent>(e)->object);
    }
}

/* Events
-------------------------------------------------- */

const EventType::Ptr ReleaseObjectEvent::EVENT_TYPE = EventTypeManager::registerNewType();
ReleaseObjectEvent::ReleaseObjectEvent(ObjectManager::Ptr objectManager, GameObject::UPtr object)
    : TargettedEvent(
        ReleaseObjectEvent::EVENT_TYPE,
        std::static_pointer_cast<EventHandler>(objectManager)
    ),
    object(move(object)) {
}

const EventType::Ptr DestroyObjectEvent::EVENT_TYPE = EventTypeManager::registerNewType();
DestroyObjectEvent::DestroyObjectEvent(ObjectManager::Ptr objectManager, GameObject* object)
    : TargettedEvent(
        DestroyObjectEvent::EVENT_TYPE,
        std::static_pointer_cast<EventHandler>(objectManager)
    ),
    object(object) {
}
