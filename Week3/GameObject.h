#pragma once

#include <memory>

#include "Derived.h"

#include "Event.h"
#include "PhysModel.h"

class GameObject : EventHandler {
public:
    using EventEmitterPtr = std::shared_ptr<EventEmitter>;
    using PhysModelPtr = std::shared_ptr<PhysModel>;

private:
    EventEmitterPtr _controller;
    PhysModelPtr _physModel;

public:
    /* Components
    -------------------- */

    // You must give an actual controller and TPhysModel
    GameObject(EventEmitterPtr controller, PhysModelPtr physModel) {
        setController(controller);
        setPhysModel(physModel);
    }

    virtual ~GameObject() {}

    virtual EventEmitter& controller() { return *_controller; }
    virtual PhysModel& physModel() { return *_physModel; }

    virtual void setController(EventEmitterPtr controller) {
        if (controller) _controller = controller;
    }
    virtual void setPhysModel(PhysModelPtr physModel) {
        if (physModel) _physModel = physModel;
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
    virtual void draw() {};

    virtual void afterFrame() {};
};
