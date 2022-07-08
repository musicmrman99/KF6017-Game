#pragma once

#include "Referencing.h"

#include "EventHandler.h"
#include "EventEmitter.h"

#include "EventEmitterObserverOf.h"
#include "ObjectEventCreator.h"

#include "NewtonianPhysModel.h"
#include "BufferedEventEmitter.h"

#include "mydrawengine.h"

class Integrity :
	public EventHandler,
	public EventEmitterObserverOf<BufferedEventEmitter>,
	public ObjectEventCreator,
	public Referencing<GameObject>
{
private:
	PictureIndex explosionImage;
	float _maxIntegrity;
	float _integrity;

	Integrity(PictureIndex explosionImage, float maxIntegrity);

public:
	using Ptr = std::shared_ptr<Integrity>;
	using UPtr = std::unique_ptr<Integrity>;

	static UPtr create(PictureIndex explosionImage, float maxIntegrity);

	class ShiftIntegrityEvent final : public Event {
	public:
		static const EventType TYPE;
		const float amount;

		ShiftIntegrityEvent(const float amount);
	};

	class ShiftIntegrityEventEmitter final : public EventEmitter {
	private:
		float amount;

	public:
		ShiftIntegrityEventEmitter(float amount);
		virtual void emit(std::queue<Event::Ptr>& events) override;
	};

	float maxIntegrity() const;
	float integrity() const;
	void shiftIntegrity(float amount);

	virtual void handle(const Event::Ptr e) override;
};
