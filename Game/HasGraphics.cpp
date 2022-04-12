#include "HasGraphics.h"

HasGraphics::HasGraphics(GraphicsModel::Ptr graphicsModel) {
    setGraphicsModel(graphicsModel);
}
HasGraphics::~HasGraphics() {}

void HasGraphics::trackGraphicsObserver(GraphicsObserver::WPtr graphicsObserver) {
    if (auto po = graphicsObserver.lock()) {
        po->updateGraphicsModel(_graphicsModel);
        _observerTracker.track(graphicsObserver);
    }
}

GraphicsModel::WPtr HasGraphics::graphicsModelWPtr() const { return _graphicsModel; }
GraphicsModel& HasGraphics::graphicsModel() const { return *_graphicsModel; }

void HasGraphics::setGraphicsModel(GraphicsModel::Ptr graphicsModel) {
    // Set the model
    if (graphicsModel) _graphicsModel = graphicsModel;
    else _graphicsModel = GraphicsModel::Ptr(new NullGraphicsModel()); // Do not allow nullptr

    // Update all observers
    bool areAnyExpired = false;
    for (GraphicsObserver::WPtr& observer : _observerTracker.getTracked()) {
        if (auto ob = observer.lock()) {
            ob->updateGraphicsModel(graphicsModel);
        } else {
            areAnyExpired = true;
        }
    }
    if (areAnyExpired) _observerTracker.dropExpired();
}

void HasGraphics::beforeDraw() {};
void HasGraphics::draw() {
    graphicsModel().draw();
}
