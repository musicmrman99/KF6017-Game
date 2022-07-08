#pragma once

#include <memory>

class Level;

// Allows bi-directional communication with the level manager.
// 
// This should be avoided unless absolutely necessary, as it creates
// a bi-directional dependency between a game object and the level,
// which creates a lot of coupling (game objects shouldn't know about
// the level). If possible, use broadcast events instead.
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
