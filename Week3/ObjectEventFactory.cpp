#include "ObjectEventFactory.h"

#include "ObjectManager.h"
#include "ObjectEvent.h"

// Depends on ObjectManager because TargettedEvent needs to know that ObjectManager is-a EventHandler.

ObjectEventFactory::ObjectEventFactory(ObjectManagerWPtr objectManager)
    : objectManager(objectManager) {
}

ObjectEventFactory::Ptr ObjectEventFactory::create(ObjectManagerWPtr objectManager) {
    return Ptr(new ObjectEventFactory(objectManager));
}

TargettedEvent::UPtr ObjectEventFactory::createObject(ObjectSpecUPtr spec) const {
    return TargettedEvent::UPtr(new TargettedEvent(
        CreateObjectEvent::create(move(spec)),
        objectManager
    ));
}

TargettedEvent::UPtr ObjectEventFactory::destroyObject(GameObjectWPtr object) const {
    return TargettedEvent::UPtr(new TargettedEvent(
        DestroyObjectEvent::create(object),
        objectManager
    ));
}

TargettedEvent::UPtr ObjectEventFactory::addController(EventEmitter::Ptr controller) const {
    return TargettedEvent::UPtr(new TargettedEvent(
        AddControllerEvent::create(controller),
        objectManager
    ));
}

TargettedEvent::UPtr ObjectEventFactory::removeController(EventEmitter::WPtr controller) const {
    return TargettedEvent::UPtr(new TargettedEvent(
        RemoveControllerEvent::create(controller),
        objectManager
    ));
}
