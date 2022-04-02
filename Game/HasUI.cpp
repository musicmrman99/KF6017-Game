#include "HasUI.h"

HasUI::HasUI(GraphicsModel::Ptr uiModel) {
    setUIModel(uiModel);
}
HasUI::~HasUI() {}

void HasUI::trackUIObserver(UIObserver::WPtr uiObserver) {
    if (auto po = uiObserver.lock()) {
        po->updateUIModel(_uiModel);
        _observerTracker.track(uiObserver);
    }
}

GraphicsModel::WPtr HasUI::uiModelWPtr() const { return _uiModel; }
GraphicsModel& HasUI::uiModel() const { return *_uiModel; }

void HasUI::setUIModel(GraphicsModel::Ptr uiModel) {
    // Set the model
    if (uiModel) _uiModel = uiModel;
    else _uiModel = GraphicsModel::Ptr(new NullGraphicsModel()); // Do not allow nullptr

    // Update all observers
    bool areAnyExpired = false;
    for (UIObserver::WPtr& observer : _observerTracker.getTracked()) {
        if (auto ob = observer.lock()) {
            ob->updateUIModel(uiModel);
        } else {
            areAnyExpired = true;
        }
    }
    if (areAnyExpired) _observerTracker.dropExpired();
}

void HasUI::beforeDrawUI() {};
void HasUI::drawUI() {
    uiModel().draw();
}
