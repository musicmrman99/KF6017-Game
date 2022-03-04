#include "ObjectManager.h"

void ObjectManager::addObject(const GameObject::Ptr gameObject) {
    objects.push_back(gameObject);
}

void ObjectManager::run() {
    // Handle actions
    for (GameObject::Ptr& object : objects) object->beforeActions();
    for (GameObject::Ptr& object : objects) object->actions();

    // Handle global events
    for (GameObject::Ptr& object : objects) object->emit(events);
    while (!events.empty()) {
        const Event& event = events.front();
        for (GameObject::Ptr& object : objects) object->handle(event);
        events.pop();
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
