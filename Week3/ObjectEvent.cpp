#include "ObjectEvent.h"

#include "ObjectSpec.h"
#include "GameObject.h"

/* Release Object Event
-------------------- */

const EventType CreateObjectEvent::TYPE;
CreateObjectEvent::CreateObjectEvent(ObjectSpecUPtr spec)
    : Event(TYPE), spec(move(spec)) {
}

CreateObjectEvent::Ptr CreateObjectEvent::create(ObjectSpecUPtr spec) {
    return UPtr(new CreateObjectEvent(move(spec)));
}

/* Destroy Object Event
-------------------- */

const EventType DestroyObjectEvent::TYPE;
DestroyObjectEvent::DestroyObjectEvent(GameObjectWPtr object)
    : Event(TYPE), object(object) {
}

DestroyObjectEvent::Ptr DestroyObjectEvent::create(GameObjectWPtr object) {
    return UPtr(new DestroyObjectEvent(object));
}

/* Add Controller Event
-------------------- */

const EventType AddControllerEvent::TYPE;
AddControllerEvent::AddControllerEvent(EventEmitter::Ptr controller)
    : Event(TYPE), controller(controller) {
}

AddControllerEvent::Ptr AddControllerEvent::create(EventEmitter::Ptr spec) {
    return UPtr(new AddControllerEvent(spec));
}

/* Destroy Object Event
-------------------- */

const EventType RemoveControllerEvent::TYPE;
RemoveControllerEvent::RemoveControllerEvent(EventEmitter::WPtr controller)
    : Event(TYPE), controller(controller) {
}

RemoveControllerEvent::Ptr RemoveControllerEvent::create(EventEmitter::WPtr controller) {
    return UPtr(new RemoveControllerEvent(controller));
}
