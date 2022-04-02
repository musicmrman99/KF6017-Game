#pragma once

#include <memory>

class CollisionModel {
public:
    using Ptr = std::shared_ptr<CollisionModel>;
    using UPtr = std::unique_ptr<CollisionModel>;
    using WPtr = std::weak_ptr<CollisionModel>;

	virtual void updateCollision() = 0;
};

class NullCollisionModel : public CollisionModel {
public:
    virtual void updateCollision() override;
};
