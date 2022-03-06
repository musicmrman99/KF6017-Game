#pragma once

#include <list>
#include <memory>

#include "GameObject.h"

class ObjectManager : public EventHandler {
private:
	std::list<GameObject::Ptr> objects;
	std::queue<Event> events;

public:
	using Ptr = std::shared_ptr<ObjectManager>;

	void addObject(GameObject* gameObject);
	void deleteObject(GameObject* gameObject);

	virtual void handle(const Event& e) override;
	void run();
};

class ObjectEvent : public TargettedEvent {
public:
	static const EventTypeVPtr RELEASE;
	static const EventTypeVPtr DESTROY;

	GameObject* object;
	ObjectEvent(ObjectManager::Ptr objectManager, const EventTypeVPtr& type, GameObject* object);
};
