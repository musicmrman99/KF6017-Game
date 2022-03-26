#include "EventEmitter.h"

// Event Emitter
EventEmitter::~EventEmitter() {}

// Null Event Emitter
void NullEventEmitter::emit(std::queue<Event::Ptr>& events) {}
