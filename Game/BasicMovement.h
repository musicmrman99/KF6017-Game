#pragma once

#include "EventHandler.h"
#include "EventEmitter.h"
#include "PhysObserverOf.h"
#include "NewtonianPhysModel.h"

class BasicMovement final :
	public EventHandler,
	public PhysObserverOf<NewtonianPhysModel>
{
private:
	float _positionalThrust;
	float _rotationalThrust;

	BasicMovement();

public:
	using Ptr = std::shared_ptr<BasicMovement>;
	using UPtr = std::unique_ptr<BasicMovement>;

	static UPtr create();

	float positionalThrust();
	float rotationalThrust();

	class MainThrustEvent final : public Event {
	public:
		static const EventType TYPE;
		MainThrustEvent();
	};
	class TurnLeftThrustEvent final : public Event {
	public:
		static const EventType TYPE;
		TurnLeftThrustEvent();
	};
	class TurnRightThrustEvent final : public Event {
	public:
		static const EventType TYPE;
		TurnRightThrustEvent();
	};

	class MainThrustEventEmitter final : public EventEmitter {
	public: virtual void emit(std::queue<Event::Ptr>& events) override;
	};
	class TurnLeftThrustEventEmitter final : public EventEmitter {
	public: virtual void emit(std::queue<Event::Ptr>& events) override;
	};
	class TurnRightThrustEventEmitter final : public EventEmitter {
	public: virtual void emit(std::queue<Event::Ptr>& events) override;
	};

	void mainThrust();
	void turnLeftThrust();
	void turnRightThrust();

	virtual void handle(const Event::Ptr e) override;
};
