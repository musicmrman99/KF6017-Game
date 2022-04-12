#pragma once

#include <vector>

#include "GameObject.h"
#include "HasPhysOf.h"
#include "HasGraphicsOf.h"
#include "PhysObserverOf.h"

#include "NewtonianPhysModel.h"
#include "ImageGraphicsModel.h"

#include "ObjectFactory.h"
#include "StarFieldSpec.h"

template <class T>
struct ParallaxLayer {
	std::vector<T> items;
	float scaler;
};

// A star field graphics model is a multi-image model with off-screen clipping
// and cross-frame positional persistance.
class StarFieldGraphicsModel : public GraphicsModel, public PhysObserverOf<NewtonianPhysModel> {
private:
	std::vector<ParallaxLayer<Vector2D>> layers;

	Vector2D size;
	float density;

	PictureIndex image;

	void distributeStars();
	void initUninitialisedStars();
	void randomiseStar(Vector2D& starPos, const float scaler);

	StarFieldGraphicsModel(
		Vector2D size,
		float density,
		PictureIndex image
	);

public:
	using Ptr = std::shared_ptr<StarFieldGraphicsModel>;
	using UPtr = std::unique_ptr<StarFieldGraphicsModel>;
	using WPtr = std::weak_ptr<StarFieldGraphicsModel>;

	static UPtr create(
		Vector2D size,
		float density,
		PictureIndex image
	);
	virtual void draw() override;

	// Sets the number of parallax layers for the star field.
	void setNumLayers(const unsigned int numLayers);

	// Sets the size and density of the star field at the same time.
	void setField(const Vector2D& size, const float density);

	// Set the size of the star field. Note that this will implicitly
	// affect the density of stars.
	void setSize(Vector2D size);

	// Set the density of the star field.
	void setDensity(float size);
};

// A star field displays a non-collidable set of N layers of stars that individually
// keep their positions as the field moves with parallax effects across the layers.
// 
// A star field is intended to be used as a background to make it easier for the
// player to see how fast they're moving in world coordinates.
// 
// The field has a set size and density (number of stars per unit of area).
class StarField :
	public GameObject,
	public HasPhysOf<NewtonianPhysModel>,
	public HasGraphicsOf<StarFieldGraphicsModel>
{
private:
	StarField(StarFieldSpec::UPtr spec);

public:
	using Ptr = std::shared_ptr<StarField>;

	static const ObjectFactory factory;

	// Sets the number of parallax layers for the star field.
	// 
	// Stars will be distributed evenly (or as close as possible) across the layers.
	// 
	// Each layer has a parallax movement multiplier of 2^((n/2) - i), where i is the layer
	// index. This means the center layer has a multiplier of 1, and it radiates out
	// exponentially base 2 from there, eg. if numLayers = 7, then the multipliers become:
	//   0.125, 0.25, 0.5, 1, 2, 4, 8.
	// 
	// See setField().
	void setNumLayers(const unsigned int numLayers);

	// Sets the maximum stars by defining density as a number in the
	// range [0-1] that acts as a proportion (multiplier) of the area
	// of the star field.
	// 
	// This function is faster than setSize() and setDensity() individually if you need to
	// set both simultaneously.
	void setField(const Vector2D& size, const float density);

	// Set the size of the star field. Note that this will implicitly
	// affect the density of stars.
	// 
	// Useful if you have to resize the screen (eg. windowed <-> fullscreen).
	// 
	// See setField().
	void setSize(Vector2D size);

	// Set the density of the star field.
	// 
	// May be useful for indicating world location? Perhaps other uses - use your imagination.
	// 
	// See setField().
	void setDensity(float size);
};
