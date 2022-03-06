#pragma once

#include <list>
#include <memory>

#include "GameObject.h"

class ObjectManager : public EventHandler {
private:
	std::list<GameObject::Ptr> objects;
	std::queue<Event::Ptr> events;

public:
	using Ptr = std::shared_ptr<ObjectManager>;

	void addObject(GameObject* gameObject);
	void deleteObject(GameObject* gameObject);

	virtual void handle(const Event::Ptr e) override;
	void run();
};

class ObjectEvent : public TargettedEvent {
public:
	static const EventType::Ptr RELEASE;
	static const EventType::Ptr DESTROY;

	GameObject* object;
	ObjectEvent(ObjectManager::Ptr objectManager, const EventType::Ptr& type, GameObject* object);
};
