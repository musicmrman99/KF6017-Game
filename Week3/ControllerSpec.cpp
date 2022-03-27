#include "ControllerSpec.h"

const ObjectType ControllerSpec::CONTROLLER;

ControllerSpec::ControllerSpec(EventEmitter::UPtr eventEmitter)
	: ObjectSpec(CONTROLLER), eventEmitter(move(eventEmitter)) {
}

ControllerSpec::UPtr ControllerSpec::create(EventEmitter::UPtr eventEmitter) {
	return UPtr(new ControllerSpec(move(eventEmitter)));
}
