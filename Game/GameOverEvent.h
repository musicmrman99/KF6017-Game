#pragma once

#include "Event.h"

// Game Over Event
class GameOverEvent final : public Event {
private:
	GameOverEvent();

public:
	using Ptr = std::shared_ptr<GameOverEvent>;
	using UPtr = std::unique_ptr<GameOverEvent>;
	using WPtr = std::weak_ptr<GameOverEvent>;

	static const EventType TYPE;
	static Ptr create();
};
