#include "ObjectEvent.h"

// Release Object Event
CreateObjectEvent::CreateObjectEvent(ObjectSpec::UPtr spec)
    : Event(), spec(move(spec)) {
}

CreateObjectEvent::UPtr CreateObjectEvent::create(ObjectSpec::UPtr spec) {
    return UPtr(new CreateObjectEvent(move(spec)));
}

// Destroy Object Event
DestroyObjectEvent::DestroyObjectEvent(GameObject* object)
    : Event(), object(object) {
}

DestroyObjectEvent::UPtr DestroyObjectEvent::create(GameObject* object) {
    return UPtr(new DestroyObjectEvent(object));
}

// Add Controller Event
AddControllerEvent::AddControllerEvent(EventEmitter::Ptr controller)
    : Event(), controller(controller) {
}

AddControllerEvent::UPtr AddControllerEvent::create(EventEmitter::Ptr spec) {
    return UPtr(new AddControllerEvent(spec));
}

// Destroy Object Event
RemoveControllerEvent::RemoveControllerEvent(EventEmitter* controller)
    : Event(), controller(controller) {
}

RemoveControllerEvent::UPtr RemoveControllerEvent::create(EventEmitter* controller) {
    return UPtr(new RemoveControllerEvent(controller));
}
