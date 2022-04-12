#pragma once

#include "Derived.h"
#include "ptrcast.h"

#include "HasUI.h"
#include "UIObserverOf.h"

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
    HasUIOf(TDGraphicsModelPtr uiModel) : HasUI(uiModel) {}

    // Note: Non-virtual hiding is intended.

    // Make changing the UI's GraphicsModel reflected in the given GraphicsModel-dependant object.
    void trackUIObserver(typename UIObserverOf<TDGraphicsModel>::WPtr uiObserver) { HasUI::trackUIObserver(uiObserver); }

    // Useful for setting the UI's GraphicsModel as an observer of another model
    TDGraphicsModelWPtr uiModelWPtr() const { return static_weak_pointer_cast<TDGraphicsModel>(HasUI::uiModelWPtr()); }
    TDGraphicsModel& uiModel() const { return static_cast<TDGraphicsModel&>(HasUI::uiModel()); }
    void setUIModel(TDGraphicsModelPtr uiModel) {
        if (uiModel) HasUI::setUIModel(uiModel);
    }
};
