#include "ObjectEvent.h"

#include "ObjectSpec.h"
#include "GameObject.h"

/* Create Object Event
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
