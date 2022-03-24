#pragma once

#include "EventEmitter.h"

class BufferedEventEmitter : public EventEmitter {
private:
	std::queue<Event::Ptr> eventsBuffer;

public:
	using UPtr = std::unique_ptr<BufferedEventEmitter>;

	virtual void enqueue(Event::Ptr e);
	virtual void emit(std::queue<Event::Ptr>& events) override;
};
