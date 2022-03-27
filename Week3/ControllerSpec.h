#pragma once

#include "ObjectSpec.h"
#include "EventEmitter.h"

class ControllerSpec final : public ObjectSpec {
private:
	ControllerSpec(EventEmitter::UPtr eventEmitter);

public:
	using Ptr = std::shared_ptr<ControllerSpec>;
	using UPtr = std::unique_ptr<ControllerSpec>;

	static const ObjectType CONTROLLER;

	EventEmitter::UPtr eventEmitter;
	static UPtr create(EventEmitter::UPtr eventEmitter);
};
