#include "ImageGraphicsModel.h"
#include "NewtonianPhysModel.h"

ImageGraphicsModel::ImageGraphicsModel(const NewtonianPhysModelPtr physModel, PictureIndex image)
	: image(image), physModel(physModel) {
}

void ImageGraphicsModel::draw() {
	try {
		MyDrawEngine* graphics = MyDrawEngine::GetInstance();
		graphics->DrawAt(physModel->pos(), image, 1.0f, physModel->rot().angle(), 0.0f);
	} catch (const std::bad_cast&) {}
}
