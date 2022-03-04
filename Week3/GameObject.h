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

    GameObject(
        EventEmitterPtr controller,
        PhysModelPtr physModel,
        GraphicsModelPtr graphicsModel,
        GraphicsModelPtr uiGraphicsModel
    );

    virtual ~GameObject();

    virtual EventEmitter& controller();
    virtual EventEmitter& controller() const;
    virtual void setController(EventEmitterPtr controller);

    virtual PhysModel& physModel();
    virtual PhysModel& physModel() const;
    virtual void setPhysModel(PhysModelPtr physModel);

    virtual GraphicsModel& graphicsModel();
    virtual GraphicsModel& graphicsModel() const;
    virtual void setGraphicsModel(GraphicsModelPtr graphicsModel);

    virtual GraphicsModel& uiGraphicsModel();
    virtual GraphicsModel& uiGraphicsModel() const;
    virtual void setUIGraphicsModel(GraphicsModelPtr uiGraphicsModel);

    /* Events
    -------------------- */

    virtual void handle(const Event& e) override;

    /* Lifecycle
    -------------------- */

    virtual void beforeActions();
    virtual void actions();

    virtual void beforePhys();
    virtual void phys();

    virtual void beforeDraw();
    virtual void draw();

    virtual void beforeDrawUI();
    virtual void drawUI();

    virtual void afterFrame();
};
