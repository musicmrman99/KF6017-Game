#pragma once

#include "MyDrawEngine.h"

#include "GraphicsModel.h"
#include "NewtonianPhysModel.h"

#include "PhysObserverOf.h"

class ImageGraphicsModel final : public GraphicsModel, public PhysObserverOf<NewtonianPhysModel> {
private:
	PictureIndex image;

public:
	using Ptr = std::shared_ptr<ImageGraphicsModel>;
	using UPtr = std::unique_ptr<ImageGraphicsModel>;
	using WPtr = std::weak_ptr<ImageGraphicsModel>;

	ImageGraphicsModel(PictureIndex image);
	virtual void draw() override;
};
