#pragma once

#include <memory>

#include "Event.h"
#include "PhysModel.h"
#include "GraphicsModel.h"

class GameObject : public EventHandler, public EventEmitter {
public:
    using Ptr = std::shared_ptr<GameObject>;
    using UPtr = std::unique_ptr<GameObject>;
    using WPtr = std::weak_ptr<GameObject>;

private:
    PhysModel::Ptr _physModel;
    GraphicsModel::Ptr _graphicsModel;
    GraphicsModel::Ptr _uiGraphicsModel;

    std::queue<Event::Ptr> eventsBuffer;

public:
    /* Components
    -------------------- */

    GameObject(
        PhysModel::Ptr physModel,
        GraphicsModel::Ptr graphicsModel,
        GraphicsModel::Ptr uiGraphicsModel
    );
    virtual ~GameObject();

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

    // Run once just after creation
    virtual void afterCreate();

    // Anything Before
    virtual void beforeFrame();

    // Event Handling
    virtual void enqueue(Event::Ptr e);

    virtual void handle(const Event::Ptr e) override;
    virtual void emit(std::queue<Event::Ptr>& events) override;

    // Run Models
    virtual void beforePhys();
    virtual void phys();

    virtual void beforeDraw();
    virtual void draw();

    virtual void beforeDrawUI();
    virtual void drawUI();

    // Anything After
    virtual void afterFrame();

    // Run once just before destruction
    virtual void beforeDestroy();
};
