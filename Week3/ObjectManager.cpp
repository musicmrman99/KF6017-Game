#include "ObjectManager.h"

void ObjectManager::addObject(const GameObjectPtr gameObject) {
    objects.push_back(gameObject);
}

void ObjectManager::run() {
    for (GameObjectPtr& object : objects) object->beforeActions();
    for (GameObjectPtr& object : objects) object->actions();

    for (GameObjectPtr& object : objects) object->beforePhys();
    for (GameObjectPtr& object : objects) object->phys();

    for (GameObjectPtr& object : objects) object->beforeDraw();
    for (GameObjectPtr& object : objects) object->draw();

    for (GameObjectPtr& object : objects) object->afterFrame();
}
