#include "GameObject.h"

#include "QueueUtils.h"

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

ObjectEventFactory::Ptr GameObject::objectEventFactory() { return _objectEventFactory; }
ObjectEventFactory::Ptr GameObject::objectEventFactory() const { return _objectEventFactory; }
void GameObject::setObjectEventFactory(ObjectEventFactory::Ptr objectEventFactory) {
    _objectEventFactory = objectEventFactory;
}

/* Lifecycle
-------------------- */

void GameObject::afterCreate() {};

void GameObject::beforeFrame() {};

// Add an event to the event buffer. This is flushed to the
// global event queue in the default implementation of emit().
void GameObject::enqueue(Event::Ptr e) {
    eventsBuffer.push(e);
}

// Handle events dispatched to this object.
void GameObject::handle(const Event::Ptr e) {}

// If you do not need to buffer events, then you should override this method.
void GameObject::emit(std::queue<Event::Ptr>& events) {
    shiftInto(eventsBuffer, events);
}

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

void GameObject::beforeDestroy() {};
