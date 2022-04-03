#pragma once

#include <memory>

class Level;

class LevelActor {
public:
    using Ptr = std::shared_ptr<LevelActor>;
    using UPtr = std::unique_ptr<LevelActor>;
    using WPtr = std::weak_ptr<LevelActor>;

	virtual ~LevelActor();

	void updateLevel(Level& level);
};
