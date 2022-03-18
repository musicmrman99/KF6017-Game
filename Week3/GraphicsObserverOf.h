#pragma once

#include "Derived.h"

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
    using GraphicsObserver::graphicsModelWPtr;
    using GraphicsObserver::graphicsModel;

    const TDGraphicsModelWPtr graphicsModelWPtr() const {
        return std::static_pointer_cast<TDGraphicsModel>(GraphicsObserver::graphicsModel());
    }
    const TDGraphicsModelPtr graphicsModel() const { return graphicsModelWPtr().lock(); }
};
