#include "ObjectEvent.h"

/* Release Object Event
-------------------- */

CreateObjectEvent::CreateObjectEvent(ObjectSpec::UPtr spec)
    : Event(), spec(move(spec)) {
}

CreateObjectEvent::Ptr CreateObjectEvent::create(ObjectSpec::UPtr spec) {
    return UPtr(new CreateObjectEvent(move(spec)));
}

/* Destroy Object Event
-------------------- */

DestroyObjectEvent::DestroyObjectEvent(GameObject::WPtr object)
    : Event(), object(object) {
}

DestroyObjectEvent::Ptr DestroyObjectEvent::create(GameObject::WPtr object) {
    return UPtr(new DestroyObjectEvent(object));
}

/* Add Controller Event
-------------------- */

AddControllerEvent::AddControllerEvent(EventEmitter::Ptr controller)
    : Event(), controller(controller) {
}

AddControllerEvent::Ptr AddControllerEvent::create(EventEmitter::Ptr spec) {
    return UPtr(new AddControllerEvent(spec));
}

/* Destroy Object Event
-------------------- */

RemoveControllerEvent::RemoveControllerEvent(EventEmitter::WPtr controller)
    : Event(), controller(controller) {
}

RemoveControllerEvent::Ptr RemoveControllerEvent::create(EventEmitter::WPtr controller) {
    return UPtr(new RemoveControllerEvent(controller));
}
