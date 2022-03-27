#pragma once

#include "ObjectSpec.h"
#include "EventEmitter.h"

class ControllerSpec final : public ObjectSpec {
private:
	ControllerSpec(EventEmitter::Ptr eventEmitter);

public:
	using Ptr = std::shared_ptr<ControllerSpec>;
	using UPtr = std::unique_ptr<ControllerSpec>;

	static const ObjectType CONTROLLER;

	EventEmitter::Ptr eventEmitter;
	static UPtr create(EventEmitter::Ptr eventEmitter);
};
