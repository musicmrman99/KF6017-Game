#pragma once

#include <vector>
#include <memory>

#include "Symbol.h"
#include "Shapes.h"

#include "PhysObserverOf.h"
#include "CollisionModel.h"
#include "NewtonianPhysModel.h"

class BasicCollisionType : public Symbol {};

class BasicCollisionModel : public CollisionModel, public PhysObserverOf<NewtonianPhysModel> {
public:
    using Ptr = std::shared_ptr<BasicCollisionModel>;
    using UPtr = std::unique_ptr<BasicCollisionModel>;
    using WPtr = std::weak_ptr<BasicCollisionModel>;

    using ShapeUPtr = std::unique_ptr<IShape2D>;
    using BasicCollisionTypeVector = std::vector<std::reference_wrapper<const BasicCollisionType>>;

private:
    ShapeUPtr shape;
    const BasicCollisionType& type;
    std::vector<std::reference_wrapper<const BasicCollisionType>> acceptedTypes;

    BasicCollisionModel(
        ShapeUPtr shape,
        const BasicCollisionType& type,
        BasicCollisionTypeVector acceptedTypes
    );

public:
    static Ptr create(
        ShapeUPtr shape,
        const BasicCollisionType& type,
        BasicCollisionTypeVector acceptedTypes
    );
    static Ptr create(
        IShape2D* shape,
        const BasicCollisionType& type,
        BasicCollisionTypeVector acceptedTypes
    );

    virtual void updateCollision() override;
    virtual const IShape2D& getShape() const;
    virtual const BasicCollisionType& getType() const;
    virtual const BasicCollisionTypeVector& getAcceptedTypes() const;
};
