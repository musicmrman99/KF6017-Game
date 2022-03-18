#pragma once

#include "Derived.h"
#include "ptrcast.h"

#include "HasGraphics.h"

// GameObject Trait: Adds the Graphics component slot and restricts it to holding a particular
// Graphics Model type.
// 
// Warning: Using objects of this type as a HasGraphics to set the GraphicsModel to something not derived
//          from TGraphicsModel, then using these functions to get it as a TGraphicsModel is equivalent to
//          performing an ordinary dangerous static_cast, and is therefore liable to cause memory
//          corruption and/or access violations.
template <class TGraphicsModel>
class HasGraphicsOf : public HasGraphics {
private:
    // A (T)ype (D)erived from (GraphicsModel)
    using TDGraphicsModel = Derived<TGraphicsModel, GraphicsModel>;
    using TDGraphicsModelPtr = std::shared_ptr<TDGraphicsModel>;
    using TDGraphicsModelWPtr = std::weak_ptr<TDGraphicsModel>;

public:
    HasGraphicsOf(TDGraphicsModelPtr graphicsModel) : HasGraphics(graphicsModel) {};

    using HasGraphics::graphicsModel;
    using HasGraphics::setGraphicsModel;

    TDGraphicsModelWPtr graphicsModelWPtr() const { return static_weak_pointer_cast<TDGraphicsModel>(HasGraphics::graphicsModelWPtr()); };
    TDGraphicsModel& graphicsModel() const { return static_cast<TDGraphicsModel&>(HasGraphics::graphicsModel()); };
    virtual void setGraphicsModel(TDGraphicsModelPtr graphicsModel) {
        if (graphicsModel) HasGraphics::setGraphicsModel(graphicsModel);
    }
};
