#pragma once

#include <list>
#include <memory>

#include "GameObject.h"

class ObjectManager {
private:
	std::list<GameObject::Ptr> objects;
	std::queue<Event> events;

public:
	void addObject(const GameObject::Ptr gameObject);

	void run();
};
