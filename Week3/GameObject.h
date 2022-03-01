#pragma once

#include <memory>

#include "Derived.h"

#include "Event.h"
#include "PhysModel.h"

template <class TPhysModel>
class GameObject : EventHandler {
private:
    using Controller = EventEmitter;
    using PhysModel = Derived<TPhysModel, PhysModel>;

    std::shared_ptr<Controller> _controller;
    std::shared_ptr<PhysModel> _physModel;

public:
    /* Components
    -------------------- */

    // You must give an actual controller and TPhysModel
    GameObject(std::shared_ptr<Controller> controller, std::shared_ptr<PhysModel> physModel) {
        setController(controller);
        setPhysModel(physModel);
    }

    virtual ~GameObject() {}

    Controller& controller() { return *_controller; }
    PhysModel& physModel() { return *_physModel; }

    void setController(std::shared_ptr<Controller> controller) {
        if (controller) _controller = controller;
    }
    void setPhysModel(std::shared_ptr<PhysModel> physModel) {
        if (physModel) _physModel = physModel;
    }

    /* Events
    -------------------- */

    virtual void handle(const Event& e) override {};

    /* Lifecycle
    -------------------- */

    virtual void beforeActions() {};
    virtual void actions() {};

    virtual void beforePhys() {};
    virtual void phys() {};

    virtual void beforeDraw() {};
    virtual void draw() {};

    virtual void afterFrame() {};
};
