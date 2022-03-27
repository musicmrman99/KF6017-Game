#pragma once

#include "GameObject.h"
#include "HasEventEmitter.h"

#include "ObjectFactory.h"
#include "ControllerSpec.h"

// Controllers are GameObjects that act as containers for an unconditional
// EventEmitter (ie. one that always emits some events whenever it is asked to
// emit, rather than emitting events in response to a condition, such as
// handling another event). Controllers have a 'frame lock' that ensures they
// only emit events once per frame to avoid an infinite loop in the event loop.
class Controller final : public GameObject, public HasEventEmitter {
private:
	bool hasEmittedThisFrame;

	Controller(ControllerSpec::UPtr spec);

public:
	using Ptr = std::shared_ptr<Controller>;
	using UPtr = std::unique_ptr<Controller>;
	using WPtr = std::weak_ptr<Controller>;
	
	static const ObjectFactory factory;

	virtual void beforeFrame() override;
	virtual void emit(std::queue<Event::Ptr>& events) override;
};
