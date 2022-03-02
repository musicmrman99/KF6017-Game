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
    GraphicsModelPtr _uiGraphicsModel;

public:
    /* Components
    -------------------- */

    GameObject(EventEmitterPtr controller, PhysModelPtr physModel, GraphicsModelPtr graphicsModel, GraphicsModelPtr uiGraphicsModel) {
        setController(controller);
        setPhysModel(physModel);
        setGraphicsModel(graphicsModel);
        setUIGraphicsModel(uiGraphicsModel);
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

    virtual GraphicsModel& uiGraphicsModel() { return *_uiGraphicsModel; }
    virtual GraphicsModel& uiGraphicsModel() const { return *_uiGraphicsModel; }
    virtual void setUIGraphicsModel(GraphicsModelPtr uiGraphicsModel) {
        if (uiGraphicsModel) _uiGraphicsModel = uiGraphicsModel;
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

    virtual void beforeDrawUI() {};
    virtual void drawUI() {
        uiGraphicsModel().draw();
    };

    virtual void afterFrame() {};
};
