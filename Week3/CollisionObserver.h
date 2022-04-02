#pragma once

#include "CollisionModel.h"

class CollisionObserver {
private:
    CollisionModel::WPtr _collisionModel;

public:
    using WPtr = std::weak_ptr<CollisionObserver>;

    virtual ~CollisionObserver() {}

    CollisionModel::WPtr collisionModelWPtr() const;
    CollisionModel::Ptr collisionModel() const;
    void updateCollisionModel(CollisionModel::WPtr collisionModel);
};
