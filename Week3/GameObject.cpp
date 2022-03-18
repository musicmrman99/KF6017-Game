#include "GameObject.h"

#include "QueueUtils.h"

/* Components
-------------------- */

GameObject::~GameObject() {}

// Add an event to the event buffer. This is flushed to the
// global event queue in the default implementation of emit().
void GameObject::enqueue(Event::Ptr e) {
    eventsBuffer.push(e);
}

/* Lifecycle
-------------------- */

void GameObject::afterCreate() {};

void GameObject::beforeFrame() {};

// Handle events dispatched to this object.
void GameObject::handle(const Event::Ptr e) {}

// If you do not need to buffer events, then you should override this method.
void GameObject::emit(std::queue<Event::Ptr>& events) {
    shiftInto(eventsBuffer, events);
}

void GameObject::afterFrame() {};

void GameObject::beforeDestroy() {};
