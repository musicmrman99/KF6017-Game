#include "GameObject.h"

#include "QueueUtils.h"

/* Components
-------------------- */

GameObject::GameObject(
    GraphicsModel::Ptr graphicsModel,
    GraphicsModel::Ptr uiGraphicsModel
) {
    setGraphicsModel(graphicsModel);
    setUIGraphicsModel(uiGraphicsModel);
}

GameObject::~GameObject() {}

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

// Add an event to the event buffer. This is flushed to the
// global event queue in the default implementation of emit().
void GameObject::enqueue(Event::Ptr e) {
    eventsBuffer.push(e);
}

/* Lifecycle
-------------------- */

void GameObject::afterCreate() {};

void GameObject::beforeFrame() {};

// Handle events dispatched to this object.
void GameObject::handle(const Event::Ptr e) {}

// If you do not need to buffer events, then you should override this method.
void GameObject::emit(std::queue<Event::Ptr>& events) {
    shiftInto(eventsBuffer, events);
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

void GameObject::beforeDestroy() {};
