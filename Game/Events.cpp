#include "Events.h"

#include "TargettedEvent.h"

Events::Ptr Events::create() {
    return Ptr(new Events());
}

// Track event emitter and handler objects

void Events::objectCreated(GameObject::Ptr object) {
    if (auto eventEmitter = std::dynamic_pointer_cast<HasEventEmitter>(object)) {
        eventEmitters.push_back(eventEmitter);
    }
    if (auto eventHandler = std::dynamic_pointer_cast<HasEventHandler>(object)) {
        eventHandlers.push_back(eventHandler);
    }
}
void Events::objectDestroyed(GameObject::Ptr object) {
    if (auto eventEmitter = std::dynamic_pointer_cast<HasEventEmitter>(object)) {
        eventEmitter->emit(events); // Emit any remaining buffered events before the object is destroyed.
        eventEmitters.remove(eventEmitter);
    }
    if (auto eventHandler = std::dynamic_pointer_cast<HasEventHandler>(object)) {
        eventHandlers.remove(eventHandler);
    }
}

// Run the lifecycle point

void Events::run() {
    for (HasEventEmitter::Ptr& object : eventEmitters) object->emit(events);
    while (!events.empty()) {
        // Handle event
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
}
