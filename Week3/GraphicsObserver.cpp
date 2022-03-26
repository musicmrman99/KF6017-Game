#include "GraphicsObserver.h"

GraphicsModel::WPtr GraphicsObserver::graphicsModelWPtr() const { return _graphicsModel; }
GraphicsModel::Ptr GraphicsObserver::graphicsModel() const { return graphicsModelWPtr().lock(); }
void GraphicsObserver::updateGraphicsModel(GraphicsModel::WPtr graphicsModel) { _graphicsModel = graphicsModel; }
