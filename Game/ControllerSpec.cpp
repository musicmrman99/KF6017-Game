#include "ControllerSpec.h"

const ObjectType ControllerSpec::CONTROLLER;

ControllerSpec::ControllerSpec(EventEmitter::Ptr eventEmitter)
	: ObjectSpec(CONTROLLER), eventEmitter(eventEmitter) {
}

ControllerSpec::UPtr ControllerSpec::create(EventEmitter::Ptr eventEmitter) {
	return UPtr(new ControllerSpec(eventEmitter));
}
