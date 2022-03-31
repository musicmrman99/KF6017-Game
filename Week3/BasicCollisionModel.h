#pragma once

#include <memory>

#include "Shapes.h"

#include "CollisionModel.h"

#include "PhysObserverOf.h"
#include "NewtonianPhysModel.h"

class BasicCollisionModel : public CollisionModel, public PhysObserverOf<NewtonianPhysModel> {
public:
    using Ptr = std::shared_ptr<BasicCollisionModel>;
    using UPtr = std::unique_ptr<BasicCollisionModel>;
    using WPtr = std::weak_ptr<BasicCollisionModel>;

    using ShapeUPtr = std::unique_ptr<IShape2D>;

private:
    ShapeUPtr shape;
    BasicCollisionModel(ShapeUPtr shape);

public:
    static Ptr create(ShapeUPtr shape);
    static Ptr create(IShape2D* shape);

    virtual void updateCollision() override;
    virtual IShape2D& getShape();
};
