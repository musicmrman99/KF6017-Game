#include "UIObserver.h"

GraphicsModel::WPtr UIObserver::uiModelWPtr() const { return _uiModel; }
GraphicsModel::Ptr UIObserver::uiModel() const { return uiModelWPtr().lock(); }
void UIObserver::updateUIModel(GraphicsModel::WPtr uiModel) { _uiModel = uiModel; }
