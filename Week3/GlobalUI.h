#pragma once

#include <vector>

#include "GameObject.h"
#include "ObjectFactory.h"

#include "GlobalUISpec.h"

// Show FPS Graphics Model
class FrameRateUIModel final : public GraphicsModel {
private:
	static constexpr auto SHOWFPS = true;

	int fpsUpdateRate = 20 - 1;
	int fpsUpdateCountdown = fpsUpdateRate;
	float fpsAvg = 0.0f;
	float fps = 0.0f;

public:
	virtual void draw() override;
};

// Global UI Graphics Model
class GlobalUIModel final : public GraphicsModel {
private:
	std::vector<GraphicsModel::Ptr> widgets;

public:
	void addWidget(GraphicsModel::Ptr widget);
	virtual void draw() override;
};

// Global UI
class GlobalUI final : public GameObject {
public:
	virtual GlobalUIModel& uiGraphicsModel() override;

	// Lifecycle

	GlobalUI(GlobalUISpec::UPtr spec);
	static const ObjectFactory factory;
};
