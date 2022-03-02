#pragma once

#include <list>
#include <memory>

#include "GameObject.h"

class ObjectManager {
public:
	using GameObjectPtr = std::shared_ptr<GameObject>;

private:
	std::list<GameObjectPtr> objects;

public:
	void addObject(const GameObjectPtr gameObject);

	void run();
};
