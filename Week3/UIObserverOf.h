#pragma once

#include "Derived.h"
#include "ptrcast.h"

#include "UIObserver.h"

// Trait of objects that depend on reading a GraphicsModel.
template <class TGraphicsModel>
class UIObserverOf : public UIObserver {
private:
    // A (T)ype (D)erived from (GraphicsModel)
    using TDGraphicsModel = Derived<TGraphicsModel, GraphicsModel>;
    using TDGraphicsModelWPtr = std::weak_ptr<TDGraphicsModel>;
    using TDGraphicsModelPtr = std::shared_ptr<TDGraphicsModel>;

public:
    using WPtr = std::weak_ptr<UIObserverOf<TDGraphicsModel>>;

    const TDGraphicsModelWPtr uiModelWPtr() const {
        return static_weak_pointer_cast<TDGraphicsModel>(UIObserver::uiModel());
    }
    const TDGraphicsModelPtr uiModel() const { return uiModelWPtr().lock(); }
};
