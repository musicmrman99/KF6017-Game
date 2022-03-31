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

GameObject::Ptr ObjectManager::createObject(ObjectSpec::UPtr spec) {
    // Create
    GameObject::Ptr object = factory.create(move(spec));
    if (!object) return nullptr;
    
    // Add to main list
    objects.push_back(object);

    // Is object event creator?
    if (auto c = std::dynamic_pointer_cast<ObjectEventCreator>(object)) {
        c->setObjectEventFactory(objectEventFactory);
    }

    // Run creation lifecycle methods
    object->afterCreate();
    if (auto eventEmitter = std::dynamic_pointer_cast<HasEventEmitter>(object)) {
        eventEmitter->emit(events); // Flush event buffer in case other game objects require an event-initialised object.
        eventEmitters.push_back(eventEmitter); //  Avoid unnecessary dynamic casts
    }

    // Add to relevant component lists
    if (auto eventHandler = std::dynamic_pointer_cast<HasEventHandler>(object)) eventHandlers.push_back(eventHandler);
    if (auto physObject = std::dynamic_pointer_cast<HasPhys>(object)) physObjects.push_back(physObject);
    if (auto graphicsObject = std::dynamic_pointer_cast<HasGraphics>(object)) graphicsObjects.push_back(graphicsObject);
    if (auto uiObject = std::dynamic_pointer_cast<HasUI>(object)) uiObjects.push_back(uiObject);

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
        // Run destruction lifecycle
        (*delObject)->beforeDestroy();
        if (auto eventEmitter = std::dynamic_pointer_cast<HasEventEmitter>(*delObject)) {
            eventEmitter->emit(events); // Emit any remaining buffered events before the object is destroyed.
            eventEmitters.remove(eventEmitter); // Avoid unnecessary dynamic casts
        }

        // Remove from relevant component lists
        if (auto eventHandler = std::dynamic_pointer_cast<HasEventHandler>(*delObject)) eventHandlers.remove(eventHandler);
        if (auto physObject = std::dynamic_pointer_cast<HasPhys>(*delObject)) physObjects.remove(physObject);
        if (auto graphicsObject = std::dynamic_pointer_cast<HasGraphics>(*delObject)) graphicsObjects.remove(graphicsObject);
        if (auto uiObject = std::dynamic_pointer_cast<HasUI>(*delObject)) uiObjects.remove(uiObject);
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
    MyDrawEngine* draw = MyDrawEngine::GetInstance();

    // Anything first
    for (GameObject::Ptr& object : objects) object->beforeFrame();

    // Handle Global Events
    for (HasEventEmitter::Ptr& object : eventEmitters) object->emit(events);
    while (!events.empty()) {
        // Handle Event
        const Event::Ptr& event = events.front();
        if (event->type == TargettedEvent::TYPE) {
            const TargettedEvent::Ptr te = std::static_pointer_cast<TargettedEvent>(event);
            if (auto ptr = te->target.lock()) ptr->handle(move(te->event));
        } else {
            for (HasEventHandler::Ptr& object : eventHandlers) object->handle(event);
        }
        events.pop();

        // Any more events to handle?
        for (HasEventEmitter::Ptr& object : eventEmitters) object->emit(events);
    }

    // Handle physics and graphics
    for (HasPhys::Ptr& object : physObjects) object->beforePhys();
    for (HasPhys::Ptr& object : physObjects) object->phys();

    // Don't keep trying to draw if drawing goes wrong on this frame
    if (draw->BeginDraw() == SUCCESS) {
    for (HasGraphics::Ptr& object : graphicsObjects) object->beforeDraw();
    for (HasGraphics::Ptr& object : graphicsObjects) object->draw();
    if (draw->EndDraw() == SUCCESS) {

    if (draw->BeginDraw() == SUCCESS) {
    for (HasUI::Ptr& object : uiObjects) object->beforeDrawUI();
    for (HasUI::Ptr& object : uiObjects) object->drawUI();
         draw->EndDraw();
    }}}
    // ... but do keep going through the lifecycle methods

    // Anything last
    for (GameObject::Ptr& object : objects) object->afterFrame();
}
