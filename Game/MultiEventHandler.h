#pragma once

#include <list>

#include "EventHandler.h"

class MultiEventHandler final : public EventHandler {
private:
	std::list<EventHandler::Ptr> handlers;

public:
	using Ptr = std::shared_ptr<MultiEventHandler>;

	static Ptr create();

	void add(EventHandler::Ptr handler);
	virtual void handle(const Event::Ptr event) override;
};
