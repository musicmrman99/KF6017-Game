#pragma once

#include "GraphicsModel.h"

class GraphicsObserver {
private:
    GraphicsModel::WPtr _graphicsModel;

public:
    using WPtr = std::weak_ptr<GraphicsObserver>;

    virtual ~GraphicsObserver() {}

    GraphicsModel::WPtr graphicsModelWPtr() const;
    GraphicsModel::Ptr graphicsModel() const;

    void updateGraphicsModel(GraphicsModel::WPtr graphicsModel);
};
