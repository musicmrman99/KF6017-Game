#include "ObjectManager.h"

/* Object Manager
-------------------------------------------------- */

void ObjectManager::addObject(GameObject* gameObject) {
    objects.push_back(GameObject::Ptr(gameObject));
}

void ObjectManager::deleteObject(GameObject* gameObject) {
    // Can use `objects.remove_if()` on a std::list since C++17
    std::list<GameObject::Ptr>::iterator removeStart = std::remove_if(
        objects.begin(), objects.end(),
        [gameObject](const GameObject::Ptr& object) {
            return object.get() == gameObject;
        }
    );
    objects.erase(removeStart, objects.end());
}

void ObjectManager::run() {
    // Handle actions
    for (GameObject::Ptr& object : objects) object->beforeActions();
    for (GameObject::Ptr& object : objects) object->actions();

    // Handle Global Events
    for (GameObject::Ptr& object : objects) object->emit(events);
    while (!events.empty()) {
        // Handle Event
        const Event* event = &events.front();
        if (const TargettedEvent* targettedEvent = dynamic_cast<const TargettedEvent*>(event)) {
            if (auto ptr = targettedEvent->target.lock()) ptr->handle(*event);
        } else {
            for (GameObject::Ptr& object : objects) object->handle(*event);
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

void ObjectManager::handle(const Event& e) {
    if (EventTypeManager::isOfType(e.type, ObjectEvent::RELEASE)) {
        addObject(static_cast<const ObjectEvent&>(e).object);
    }
    else if (EventTypeManager::isOfType(e.type, ObjectEvent::DESTROY)) {
        deleteObject(static_cast<const ObjectEvent&>(e).object);
    }
}

/* Events
-------------------------------------------------- */

const EventTypeVPtr ObjectEvent::RELEASE = EventTypeManager::registerNewType();
const EventTypeVPtr ObjectEvent::DESTROY = EventTypeManager::registerNewType();

ObjectEvent::ObjectEvent(ObjectManager::Ptr objectManager, const EventTypeVPtr& type, GameObject* object)
    : TargettedEvent(type, std::static_pointer_cast<EventHandler>(objectManager)), object(object) {
}
