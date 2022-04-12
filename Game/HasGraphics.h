#pragma once

#include <memory>

#include "Tracker.h"

#include "GraphicsModel.h"
#include "GraphicsObserver.h"

/*
GameObject Trait: Adds the Graphics component slot.

This includes:
- The GraphicsModel (use graphicsModel()/setGraphicsModel() for get/set)
- An observer tracker for the GraphicsModel dependants (use trackGraphicsObserver() to add a dependant object)
- The Graphics lifecycle methods (beforeDraw() and draw())
*/
class HasGraphics {
private:
    Tracker<GraphicsObserver> _observerTracker;
    GraphicsModel::Ptr _graphicsModel;

public:
    using Ptr = std::shared_ptr<HasGraphics>;
    using UPtr = std::unique_ptr<HasGraphics>;
    using WPtr = std::weak_ptr<HasGraphics>;

    // Initialise the Graphics component slot
    HasGraphics(GraphicsModel::Ptr graphicsModel);

    virtual ~HasGraphics();

    // Make changing the GraphicsModel reflected in the given GraphicsModel-dependant object.
    void trackGraphicsObserver(GraphicsObserver::WPtr graphicsObserver);

    GraphicsModel::WPtr graphicsModelWPtr() const;           // Useful for setting the GraphicsModel as an observer of another model
    GraphicsModel& graphicsModel() const;                    // Get the GraphicsModel
    void setGraphicsModel(GraphicsModel::Ptr graphicsModel); // Set the GraphicsModel (and update all observers)

    virtual void beforeDraw(); // Run the "beforeDraw" lifecycle method
    virtual void draw();       // Run the "draw" lifecycle method
};
