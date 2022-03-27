#include "Controller.h"

#include "ptrcast.h"

Controller::Controller(ControllerSpec::UPtr spec) :
    HasEventEmitter(move(spec->eventEmitter)),
    hasEmittedThisFrame(false) {
}

const ObjectFactory Controller::factory = [](ObjectSpec::UPtr spec) {
    return Controller::Ptr(new Controller(static_unique_pointer_cast<ControllerSpec>(move(spec))));
};

void Controller::beforeFrame() {
    hasEmittedThisFrame = false;
}

void Controller::emit(std::queue<Event::Ptr>& events) {
    if (!hasEmittedThisFrame) {
        HasEventEmitter::emit(events);
        hasEmittedThisFrame = true;
    }
}
