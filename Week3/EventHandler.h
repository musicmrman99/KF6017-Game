#pragma once

#include "Event.h"

// Event Handler
class EventHandler : public Symbol {
public:
	using Ptr = std::shared_ptr<EventHandler>;
	using UPtr = std::unique_ptr<EventHandler>;
	using WPtr = std::weak_ptr<EventHandler>;

	virtual void handle(const Event::Ptr e) = 0;
	virtual ~EventHandler();
};

// Null Event Handler
class NullEventHandler final : public EventHandler {
public:
	using Ptr = std::shared_ptr<NullEventHandler>;
	using UPtr = std::unique_ptr<NullEventHandler>;
	using WPtr = std::weak_ptr<NullEventHandler>;

	virtual void handle(const Event::Ptr e) override;
};
