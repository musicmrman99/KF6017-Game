#pragma once

#include "Derived.h"
#include "ptrcast.h"

#include "HasUI.h"

// GameObject Trait: Adds the UI component slot and restricts it to holding a particular
// Graphics Model type.
// 
// Warning: Using objects of this type as a HasUI to set the GraphicsModel to something not derived
//          from TGraphicsModel, then using these functions to get it as a TGraphicsModel is equivalent to
//          performing an ordinary dangerous static_cast, and is therefore liable to cause memory
//          corruption and/or access violations.
template <class TGraphicsModel>
class HasUIOf : public HasUI {
private:
    // A (T)ype (D)erived from (GraphicsModel)
    using TDGraphicsModel = Derived<TGraphicsModel, GraphicsModel>;
    using TDGraphicsModelPtr = std::shared_ptr<TDGraphicsModel>;
    using TDGraphicsModelWPtr = std::weak_ptr<TDGraphicsModel>;

public:
    HasUIOf(TDGraphicsModelPtr uiModel) : HasUI(uiModel) {};

    using HasUI::uiModel;
    using HasUI::setUIModel;

    TDGraphicsModelWPtr uiModelWPtr() const { return static_weak_pointer_cast<TDGraphicsModel>(HasUI::uiModelWPtr()); };
    TDGraphicsModel& uiModel() const { return static_cast<TDGraphicsModel&>(HasUI::uiModel()); };
    void setUIModel(TDGraphicsModelPtr uiModel) {
        if (uiModel) HasUI::setUIModel(uiModel);
    }
};
