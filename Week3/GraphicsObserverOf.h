#pragma once

#include "Derived.h"
#include "ptrcast.h"

#include "GraphicsObserver.h"

// Trait of objects that depend on reading a GraphicsModel.
template <class TGraphicsModel>
class GraphicsObserverOf : public GraphicsObserver {
private:
    // A (T)ype (D)erived from (GraphicsModel)
    using TDGraphicsModel = Derived<TGraphicsModel, GraphicsModel>;
    using TDGraphicsModelWPtr = std::weak_ptr<TDGraphicsModel>;
    using TDGraphicsModelPtr = std::shared_ptr<TDGraphicsModel>;

public:
    using WPtr = std::weak_ptr<GraphicsObserverOf<TDGraphicsModel>>;

    const TDGraphicsModelWPtr graphicsModelWPtr() const {
        return static_weak_pointer_cast<TDGraphicsModel>(GraphicsObserver::graphicsModel());
    }
    const TDGraphicsModelPtr graphicsModel() const { return graphicsModelWPtr().lock(); }
};
