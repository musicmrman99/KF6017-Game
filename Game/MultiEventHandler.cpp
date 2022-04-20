#include "MultiEventHandler.h"

MultiEventHandler::Ptr MultiEventHandler::create() {
	return Ptr(new MultiEventHandler());
}

void MultiEventHandler::add(EventHandler::Ptr handler) {
	handlers.push_back(handler);
}

void MultiEventHandler::handle(const Event::Ptr event) {
	for (EventHandler::Ptr& handler : handlers) handler->handle(event);
}
