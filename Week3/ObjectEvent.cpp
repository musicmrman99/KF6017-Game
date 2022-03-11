#include "ObjectEvent.h"

// Release Object Event
CreateObjectEvent::CreateObjectEvent(ObjectSpec::UPtr spec)
    : Event(), spec(move(spec)) {
}

CreateObjectEvent::UPtr CreateObjectEvent::create(ObjectSpec::UPtr spec) {
    return CreateObjectEvent::UPtr(new CreateObjectEvent(move(spec)));
}

// Destroy Object Event
DestroyObjectEvent::DestroyObjectEvent(GameObject* object)
    : Event(), object(object) {
}

DestroyObjectEvent::UPtr DestroyObjectEvent::create(GameObject* object) {
    return DestroyObjectEvent::UPtr(new DestroyObjectEvent(object));
}
