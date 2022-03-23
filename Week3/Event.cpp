#include "Event.h"

// Event
Event::Event(const EventType& type) : type(type) {}
Event::Event(const Event::Ptr& ptr) : type(ptr->type) {}
Event::~Event() {}
