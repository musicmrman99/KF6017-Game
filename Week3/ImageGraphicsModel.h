#pragma once

#include "MyDrawEngine.h"

#include "GraphicsModel.h"
#include "NewtonianPhysModel.h"

class ImageGraphicsModel final : public GraphicsModel {
private:
	NewtonianPhysModel::Ptr physModel;
	PictureIndex image;

public:
	ImageGraphicsModel(const NewtonianPhysModel::Ptr physModel, PictureIndex image);

	void setPhysModel(const NewtonianPhysModel::Ptr physModel);
	virtual void draw() override;
};
