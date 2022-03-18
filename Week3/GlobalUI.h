#pragma once

#include <vector>
#include <memory>

#include "GameObject.h"
#include "HasUIOf.h"
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
	using UPtr = std::unique_ptr<GlobalUIModel>;

	void addWidget(GraphicsModel::Ptr widget);
	virtual void draw() override;
};

// Global UI
class GlobalUI final : public GameObject, public HasUIOf<GlobalUIModel> {
public:
	GlobalUI(GlobalUISpec::UPtr spec);
	static const ObjectFactory factory;
};
