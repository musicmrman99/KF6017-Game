#pragma once

#include <list>

#include "LifecyclePoint.h"

#include "HasGraphics.h"
#include "HasUI.h"

class Graphics : public LifecyclePoint {
private:
	std::list<HasGraphics::Ptr> graphicsObjects;
	std::list<HasUI::Ptr> uiObjects;

public:
	using Ptr = std::shared_ptr<Graphics>;

	static Ptr create();

	virtual void objectCreated(GameObject::Ptr object) override;
	virtual void objectDestroyed(GameObject::Ptr object) override;
	virtual void run() override;
};
