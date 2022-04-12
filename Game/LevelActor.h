#pragma once

#include <memory>

class Level;

class LevelActor {
private:
    using LevelPtr = std::shared_ptr<Level>;

    LevelPtr _level;

public:
    using Ptr = std::shared_ptr<LevelActor>;
    using UPtr = std::unique_ptr<LevelActor>;
    using WPtr = std::weak_ptr<LevelActor>;

	virtual ~LevelActor();

	void setLevel(LevelPtr level);
    LevelPtr level();
};
