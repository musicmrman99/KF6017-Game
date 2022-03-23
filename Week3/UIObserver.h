#pragma once

#include "GraphicsModel.h"

class UIObserver {
private:
    GraphicsModel::WPtr _uiModel;

public:
    using WPtr = std::weak_ptr<UIObserver>;

    virtual ~UIObserver() {}

    GraphicsModel::WPtr uiModelWPtr() const;
    GraphicsModel::Ptr uiModel() const;
    void updateUIModel(GraphicsModel::WPtr uiModel);
};
