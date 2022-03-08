#pragma once

#include "MyDrawEngine.h"

#include "GraphicsModel.h"
#include "NewtonianPhysModel.h"

class ImageGraphicsModel final : public GraphicsModel {
private:
	NewtonianPhysModel::Ptr physModel;
	PictureIndex image;

public:
	using Ptr = std::shared_ptr<ImageGraphicsModel>;
	using UPtr = std::unique_ptr<ImageGraphicsModel>;
	using WPtr = std::weak_ptr<ImageGraphicsModel>;

	ImageGraphicsModel(const NewtonianPhysModel::Ptr physModel, PictureIndex image);

	void setPhysModel(const NewtonianPhysModel::Ptr physModel);
	virtual void draw() override;
};
