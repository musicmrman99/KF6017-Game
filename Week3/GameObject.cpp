#include "GameObject.h"

/* Components
-------------------- */

GameObject::GameObject(
    EventEmitterPtr controller,
    PhysModelPtr physModel,
    GraphicsModelPtr graphicsModel,
    GraphicsModelPtr uiGraphicsModel
) {
    setController(controller);
    setPhysModel(physModel);
    setGraphicsModel(graphicsModel);
    setUIGraphicsModel(uiGraphicsModel);
}

GameObject::~GameObject() {}

EventEmitter& GameObject::controller() { return *_controller; }
EventEmitter& GameObject::controller() const { return *_controller; }
void GameObject::setController(EventEmitterPtr controller) {
    if (controller) _controller = controller;
}

PhysModel& GameObject::physModel() { return *_physModel; }
PhysModel& GameObject::physModel() const { return *_physModel; }
void GameObject::setPhysModel(PhysModelPtr physModel) {
    if (physModel) _physModel = physModel;
}

GraphicsModel& GameObject::graphicsModel() { return *_graphicsModel; }
GraphicsModel& GameObject::graphicsModel() const { return *_graphicsModel; }
void GameObject::setGraphicsModel(GraphicsModelPtr graphicsModel) {
    if (graphicsModel) _graphicsModel = graphicsModel;
}

GraphicsModel& GameObject::uiGraphicsModel() { return *_uiGraphicsModel; }
GraphicsModel& GameObject::uiGraphicsModel() const { return *_uiGraphicsModel; }
void GameObject::setUIGraphicsModel(GraphicsModelPtr uiGraphicsModel) {
    if (uiGraphicsModel) _uiGraphicsModel = uiGraphicsModel;
}

/* Events
-------------------- */

void GameObject::handle(const Event& e) {};

/* Lifecycle
-------------------- */

void GameObject::beforeActions() {};
void GameObject::actions() {
    controller().emit(events);
    while (!events.empty()) {
        const Event& event = events.front();
        handle(event);
        events.pop();
    }
}

void GameObject::emit(std::queue<Event>& globalEvents) {}

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
