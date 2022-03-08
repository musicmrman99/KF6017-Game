#pragma once

#include <memory>

#include "Derived.h"

#include "Event.h"
#include "PhysModel.h"
#include "GraphicsModel.h"

class GameObject : public EventHandler, public EventEmitter {
public:
    using Ptr = std::shared_ptr<GameObject>;
    using UPtr = std::unique_ptr<GameObject>;
    using WPtr = std::weak_ptr<GameObject>;

private:
    EventEmitter::Ptr _controller;
    PhysModel::Ptr _physModel;
    GraphicsModel::Ptr _graphicsModel;
    GraphicsModel::Ptr _uiGraphicsModel;

    std::queue<Event::Ptr> events;

public:
    /* Components
    -------------------- */

    GameObject(
        EventEmitter::Ptr controller,
        PhysModel::Ptr physModel,
        GraphicsModel::Ptr graphicsModel,
        GraphicsModel::Ptr uiGraphicsModel
    );
    virtual ~GameObject();

    virtual EventEmitter& controller();
    virtual EventEmitter& controller() const;
    virtual void setController(EventEmitter::Ptr controller);

    virtual PhysModel& physModel();
    virtual PhysModel& physModel() const;
    virtual void setPhysModel(PhysModel::Ptr physModel);

    virtual GraphicsModel& graphicsModel();
    virtual GraphicsModel& graphicsModel() const;
    virtual void setGraphicsModel(GraphicsModel::Ptr graphicsModel);

    virtual GraphicsModel& uiGraphicsModel();
    virtual GraphicsModel& uiGraphicsModel() const;
    virtual void setUIGraphicsModel(GraphicsModel::Ptr uiGraphicsModel);

    /* Lifecycle
    -------------------- */

    // Action Handling
    virtual void beforeActions();
    virtual void actions();

    // Event Handling
    virtual void handle(const Event::Ptr e) override;
    virtual void emit(std::queue<Event::Ptr>& events) override;

    // Run other Model Phase
    virtual void beforePhys();
    virtual void phys();

    virtual void beforeDraw();
    virtual void draw();

    virtual void beforeDrawUI();
    virtual void drawUI();

    virtual void afterFrame();
};
