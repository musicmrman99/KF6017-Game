#include "BufferedEventEmitter.h"

#include "QueueUtils.h"

// Add an event to the event buffer. This is flushed to the
// global event queue in the default implementation of emit().
void BufferedEventEmitter::enqueue(Event::Ptr e) {
    eventsBuffer.push(e);
}

// If you do not need to buffer events, then you should override this method.
void BufferedEventEmitter::emit(std::queue<Event::Ptr>& events) {
    shiftInto(eventsBuffer, events);
}
