#pragma once

#include <memory>

#include "Event.h"
#include "EventHandler.h"
#include "EventEmitter.h"
#include "ObjectEventCreator.h"

#include "PhysModel.h"
#include "GraphicsModel.h"

class GameObject :
    public EventHandler,
    public EventEmitter,
    public ObjectEventCreator
{
public:
    using Ptr = std::shared_ptr<GameObject>;
    using UPtr = std::unique_ptr<GameObject>;
    using WPtr = std::weak_ptr<GameObject>;

private:
    std::queue<Event::Ptr> eventsBuffer;

public:
    /* Components
    -------------------- */

    virtual ~GameObject();

    // Events
    
    virtual void enqueue(Event::Ptr e);

    /* Lifecycle
    -------------------- */

    // Run once just after creation
    virtual void afterCreate();

    // Anything Before
    virtual void beforeFrame();

    // Event Handling
    virtual void handle(const Event::Ptr e) override;
    virtual void emit(std::queue<Event::Ptr>& events) override;

    // Anything After
    virtual void afterFrame();

    // Run once just before destruction
    virtual void beforeDestroy();
};
