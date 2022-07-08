#pragma once

#include "EventHandler.h"
#include "EventEmitter.h"

#include "PhysObserverOf.h"
#include "EventEmitterObserverOf.h"
#include "ObjectEventCreator.h"

#include "NewtonianPhysModel.h"
#include "BufferedEventEmitter.h"

#include "mydrawengine.h"

class SprayAttack final :
	public EventHandler,
	public PhysObserverOf<NewtonianPhysModel>,
	public EventEmitterObserverOf<BufferedEventEmitter>,
	public ObjectEventCreator
{
private:
	PictureIndex bulletImage;
	float bulletDamage;
	float _offsetAngle;

	SprayAttack(PictureIndex bulletImage, float bulletDamage);

public:
	using Ptr = std::shared_ptr<SprayAttack>;
	using UPtr = std::unique_ptr<SprayAttack>;

	static UPtr create(PictureIndex bulletImage, float bulletDamage);

	class RotateFiringAngleEvent final : public Event {
	public:
		static const EventType TYPE;
		const float amount;

		RotateFiringAngleEvent(const float amount);
	};

	class RotateFiringAngleEventEmitter final : public EventEmitter {
	private:
		float amount;

	public:
		RotateFiringAngleEventEmitter(float amount);
		virtual void emit(std::queue<Event::Ptr>& events) override;
	};

	float offsetAngle() const;
	void rotateOffsetAngle(float amount);

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
