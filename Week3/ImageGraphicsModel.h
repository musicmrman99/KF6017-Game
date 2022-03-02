#pragma once

#include "MyDrawEngine.h"

#include "GraphicsModel.h"
#include "NewtonianPhysModel.h"

class ImageGraphicsModel : public GraphicsModel {
private:
	using NewtonianPhysModelPtr = std::shared_ptr<NewtonianPhysModel>;

	const NewtonianPhysModelPtr physModel;

	PictureIndex image;

public:
	ImageGraphicsModel(const NewtonianPhysModelPtr physModel, PictureIndex image);

	virtual void draw() override;
};
