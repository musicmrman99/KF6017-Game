#include "Controller.h"

#include "ptrcast.h"
#include "Referencing.h"

Controller::Controller(ControllerSpec::UPtr spec) :
    HasEventEmitter(move(spec->eventEmitter)),
    hasEmittedThisFrame(false) {
}

const ObjectFactory Controller::factory = [](ObjectSpec::UPtr spec) {
    Controller::Ptr controller = Controller::Ptr(new Controller(static_unique_pointer_cast<ControllerSpec>(move(spec))));
    if (auto referencingController = dynamic_cast<Referencing<Controller>*>(&controller->eventEmitter())) {
        referencingController->setRef(controller);                     // DEPENDS: Controller?
    }
    return controller;
};

void Controller::setObjectEventFactory(ObjectEventFactory::Ptr objectEventFactory) {
    // Controller doesn't need a copy of objectEventFactory itself.
    if (auto objectEventCreator = dynamic_cast<ObjectEventCreator*>(&eventEmitter())) {
        objectEventCreator->setObjectEventFactory(objectEventFactory); // DEPENDS: ObjectEventFactory?
    }
}

void Controller::beforeFrame() {
    hasEmittedThisFrame = false;
}

void Controller::emit(std::queue<Event::Ptr>& events) {
    if (!hasEmittedThisFrame) {
        HasEventEmitter::emit(events);
        hasEmittedThisFrame = true;
    }
}
