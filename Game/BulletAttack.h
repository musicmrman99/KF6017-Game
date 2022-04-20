#pragma once

#include "EventHandler.h"
#include "EventEmitter.h"

#include "PhysObserverOf.h"
#include "EventEmitterObserverOf.h"
#include "ObjectEventCreator.h"

#include "NewtonianPhysModel.h"
#include "BufferedEventEmitter.h"

#include "mydrawengine.h"

class BulletAttack final :
	public EventHandler,
	public PhysObserverOf<NewtonianPhysModel>,
	public EventEmitterObserverOf<BufferedEventEmitter>,
	public ObjectEventCreator
{
private:
	PictureIndex bulletImage;

	BulletAttack(PictureIndex bulletImage);

public:
	using Ptr = std::shared_ptr<BulletAttack>;
	using UPtr = std::unique_ptr<BulletAttack>;

	static UPtr create(PictureIndex bulletImage);

	class FireEvent final : public Event {
	public:
		static const EventType TYPE;
		FireEvent();
	};

	class FireEventEmitter final : public EventEmitter {
	public: virtual void emit(std::queue<Event::Ptr>& events) override;
	};

	virtual void fire();

	virtual void handle(const Event::Ptr e) override;
};
