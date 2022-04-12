#pragma once

#include <memory>

#include "Tracker.h"

#include "GraphicsModel.h"
#include "UIObserver.h"

/*
GameObject Trait: Adds the UI component slot.

This includes:
- The GraphicsModel (use uiModel()/setUIModel() for get/set)
- An observer tracker for the GraphicsModel dependants (use trackUIObserver() to add a dependant object)
- The UI lifecycle methods (beforeDrawUI() and drawUI())
*/
class HasUI {
private:
    Tracker<UIObserver> _observerTracker;
    GraphicsModel::Ptr _uiModel;

public:
    using Ptr = std::shared_ptr<HasUI>;
    using UPtr = std::unique_ptr<HasUI>;
    using WPtr = std::weak_ptr<HasUI>;

    // Initialise the UI component slot
    HasUI(GraphicsModel::Ptr uiModel);

    virtual ~HasUI();

    // Make changing the GraphicsModel reflected in the given GraphicsModel-dependant object.
    void trackUIObserver(UIObserver::WPtr uiObserver);

    GraphicsModel::WPtr uiModelWPtr() const;     // Useful for setting the GraphicsModel as an observer of another model
    GraphicsModel& uiModel() const;              // Get the GraphicsModel
    void setUIModel(GraphicsModel::Ptr uiModel); // Set the GraphicsModel (and update all observers)

    virtual void beforeDrawUI(); // Run the "beforeDraw" lifecycle method
    virtual void drawUI();       // Run the "draw" lifecycle method
};
