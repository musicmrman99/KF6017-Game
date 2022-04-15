#pragma once

// Dependencies
#include <memory>
#include "MyDrawEngine.h"

// Traits
#include "Ship.h"
#include "HasEventHandlerOf.h"

// Creation
#include "ObjectFactory.h"
#include "FighterShipSpec.h"

/* Event Handler
-------------------- */

class FighterShipEventHandler final :
	public EventHandler,
	public PhysObserverOf<NewtonianPhysModel>,
	public EventEmitterObserverOf<BufferedEventEmitter>,
	public ObjectEventCreator
{
private:
	// Gameplay
	float engineThrust;
	float rotateThrust;

	PictureIndex bulletImage;

public:
	using UPtr = std::unique_ptr<FighterShipEventHandler>;

	FighterShipEventHandler(FighterShipSpec::Ptr spec);

	virtual void handle(const Event::Ptr e) override;

	// Movement

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

	// Attack

	class FireEvent final : public Event {
	public:
		static const EventType TYPE;
		FireEvent();
	};

	class FireEventEmitter final : public EventEmitter {
	public: virtual void emit(std::queue<Event::Ptr>& events) override;
	};

	void fire();
};

/* Player Ship
-------------------- */

class FighterShip final :
	public Ship,
	public HasEventHandlerOf<FighterShipEventHandler>
{
public:
	using Ptr = std::shared_ptr<FighterShip>;

	// Lifecycle

	FighterShip(FighterShipSpec::Ptr spec);
	static const ObjectFactory factory;

	// Delegate event factory dependency to components that need it.
	virtual void setObjectEventFactory(ObjectEventFactory::Ptr objectEventFactory) override;
};
