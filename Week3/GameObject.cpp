#include "GameObject.h"

/* Components
-------------------- */

GameObject::GameObject(
    PhysModel::Ptr physModel,
    GraphicsModel::Ptr graphicsModel,
    GraphicsModel::Ptr uiGraphicsModel
) {
    setPhysModel(physModel);
    setGraphicsModel(graphicsModel);
    setUIGraphicsModel(uiGraphicsModel);
}

GameObject::~GameObject() {}

PhysModel& GameObject::physModel() { return *_physModel; }
PhysModel& GameObject::physModel() const { return *_physModel; }
void GameObject::setPhysModel(PhysModel::Ptr physModel) {
    if (physModel) _physModel = physModel;
}

GraphicsModel& GameObject::graphicsModel() { return *_graphicsModel; }
GraphicsModel& GameObject::graphicsModel() const { return *_graphicsModel; }
void GameObject::setGraphicsModel(GraphicsModel::Ptr graphicsModel) {
    if (graphicsModel) _graphicsModel = graphicsModel;
}

GraphicsModel& GameObject::uiGraphicsModel() { return *_uiGraphicsModel; }
GraphicsModel& GameObject::uiGraphicsModel() const { return *_uiGraphicsModel; }
void GameObject::setUIGraphicsModel(GraphicsModel::Ptr uiGraphicsModel) {
    if (uiGraphicsModel) _uiGraphicsModel = uiGraphicsModel;
}

/* Lifecycle
-------------------- */

void GameObject::beforeFrame() {};

void GameObject::handle(const Event::Ptr e) {}
void GameObject::emit(std::queue<Event::Ptr>& globalEvents) {}

void GameObject::beforePhys() {};
void GameObject::phys() {
    physModel().run();
}

void GameObject::beforeDraw() {};
void GameObject::draw() {
    graphicsModel().draw();
};

void GameObject::beforeDrawUI() {};
void GameObject::drawUI() {
    uiGraphicsModel().draw();
};

void GameObject::afterFrame() {};
