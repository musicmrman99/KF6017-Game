#pragma once

#include <memory>

#include "Derived.h"

#include "Event.h"
#include "PhysModel.h"
#include "GraphicsModel.h"

class GameObject : EventHandler {
public:
    using EventEmitterPtr = std::shared_ptr<EventEmitter>;
    using PhysModelPtr = std::shared_ptr<PhysModel>;
    using GraphicsModelPtr = std::shared_ptr<GraphicsModel>;

private:
    EventEmitterPtr _controller;
    PhysModelPtr _physModel;
    GraphicsModelPtr _graphicsModel;

public:
    /* Components
    -------------------- */

    // You must give an actual controller and TPhysModel
    GameObject(EventEmitterPtr controller, PhysModelPtr physModel, GraphicsModelPtr graphModel) {
        setController(controller);
        setPhysModel(physModel);
        setGraphicsModel(graphModel);
    }

    virtual ~GameObject() {}

    virtual EventEmitter& controller() { return *_controller; }
    virtual EventEmitter& controller() const { return *_controller; }
    virtual void setController(EventEmitterPtr controller) {
        if (controller) _controller = controller;
    }

    virtual PhysModel& physModel() { return *_physModel; }
    virtual PhysModel& physModel() const { return *_physModel; }
    virtual void setPhysModel(PhysModelPtr physModel) {
        if (physModel) _physModel = physModel;
    }

    virtual GraphicsModel& graphicsModel() { return *_graphicsModel; }
    virtual GraphicsModel& graphicsModel() const { return *_graphicsModel; }
    virtual void setGraphicsModel(GraphicsModelPtr graphicsModel) {
        if (graphicsModel) _graphicsModel = graphicsModel;
    }

    /* Events
    -------------------- */

    virtual void handle(const Event& e) override {};

    /* Lifecycle
    -------------------- */

    virtual void beforeActions() {};
    void actions() {
        static std::queue<Event> events;

        controller().emit(events);
        while (!events.empty()) {
            const Event& event = events.front();
            handle(event);
            events.pop();
        }
    }

    virtual void beforePhys() {};
    void phys() {
        physModel().run();
    }

    virtual void beforeDraw() {};
    virtual void draw() {
        graphicsModel().draw();
    };

    virtual void afterFrame() {};
};
