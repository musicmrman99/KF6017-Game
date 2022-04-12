#include "StarField.h"

#include "NewtonianPhysModel.h"

/* Graphics Model
-------------------------------------------------- */

StarFieldGraphicsModel::StarFieldGraphicsModel(
    Vector2D size,
    float density,
    PictureIndex image
) :
    size(size),
    density(density),
    image(image) {
}

// You must call setNumLayers() after this call
StarFieldGraphicsModel::UPtr StarFieldGraphicsModel::create(
    Vector2D size, // The size of the field
    float density, // The fraction of stars in the field's area (across all layers)
    PictureIndex image
) {
    return UPtr(new StarFieldGraphicsModel(size, density, image));
}

void StarFieldGraphicsModel::draw() {
    // Cache some getter values
    MyDrawEngine* graphics = MyDrawEngine::GetInstance();
    Rectangle2D viewport = graphics->GetViewport();
    Vector2D fieldPos = physModel().pos();
    float fieldAngle = physModel().rot().angle();

    try {
        Vector2D displayedPos;
        for (size_t layerIndex = 0; layerIndex < layers.size(); layerIndex++) {
            ParallaxLayer<Vector2D>& layer = layers[layerIndex];
            for (Vector2D& starPos : layer.items) {
                // Calculate displayed position
                displayedPos = fieldPos + (starPos - fieldPos) * layer.scaler;

                // If off-screen, then reposition
                if (!graphics->GetViewport().intersects(displayedPos)) {
                    randomiseStar(starPos, layer.scaler);
                }

                // Draw
                graphics->DrawAt(displayedPos, image, 1.0f - 0.15f * layerIndex, fieldAngle, 0.0f);
            }
        }
        
    } catch (const std::bad_cast&) {}
}

void StarFieldGraphicsModel::setNumLayers(const unsigned int numLayers) {
    layers.resize(numLayers);

    /* Go from middle layer (2^1) outwards, setting the value to 2^(mid - i), eg.
     *   numLayers = 7;  mid = 3 (ie. 3.5 tuncated)
     *   exp calc:   (3 - 6)                        (3 - 0)
     *   exponents: |    -3 |   -2 |  -1 | 0 | 1 | 2 | 3 |
     *   values:    | 0.125 | 0.25 | 0.5 | 1 | 2 | 4 | 8 |
     */
    int mid = static_cast<int>(numLayers / 2);
    for (size_t i = 0; i < layers.size(); i++) {
        layers[i].scaler = static_cast<float>(exp2(mid - static_cast<int>(i)));
    }
    
    distributeStars();
    initUninitialisedStars();
}

void StarFieldGraphicsModel::setField(const Vector2D& size, const float density) {
    this->size = size;
    this->density = density;

    distributeStars();
    initUninitialisedStars();
}

void StarFieldGraphicsModel::setSize(Vector2D size) {
    setField(size, this->density);
}

void StarFieldGraphicsModel::setDensity(float density) {
    setField(this->size, density);
}

/* Utilities
-------------------- */

// Distribute the stars across the layers
void StarFieldGraphicsModel::distributeStars() {
    // Truncate the results where applicable
    unsigned int maxStars = static_cast<unsigned int>(      // Only needs to be approximate
        size.XValue * size.YValue * density
    );
    unsigned int starsPerLayer = static_cast<unsigned int>( // You can't have 3.3 stars on a layer
        maxStars / layers.size()
    );
    unsigned int remainingStars = maxStars % layers.size();
    for (size_t i = 0; i < layers.size(); i++) {
        layers[i].items.resize(starsPerLayer + (i < remainingStars ? 1 : 0));
    }
}

// Set the position of all default-initialised stars to a random on-screen position.
// All initialised stars are left as-is.
void StarFieldGraphicsModel::initUninitialisedStars() {
    // Note: std::vector::resize() will default-initialise a Vector2D, placing it at
    //       (0, 0). Detecting and replacing (0, 0) means that no star can be at (0, 0)
    //       exactly, but users likely won't notice (it's one pixel out of millions on
    //       modern displays).

    for (ParallaxLayer<Vector2D>& layer : layers) {
        for (Vector2D& starPos : layer.items) {
            if (starPos.XValue == 0 && starPos.YValue == 0) {
                randomiseStar(starPos, layer.scaler);
            }
        }
    }
}

void StarFieldGraphicsModel::randomiseStar(Vector2D& starPos, const float scaler) {
    // Random on-screen position, adjusting for parallax scale.
    // Note: The value stored is the actual world coordinates of the star,
    //       which is then 'pulled towards/pushed away from' the star field's
    //       position to create the parallax effect. Hence, this formula is the
    //       inverse of the drawing formula.
    unsigned int xMax = static_cast<unsigned int>(size.XValue / scaler); // Only need to be approximate
    unsigned int yMax = static_cast<unsigned int>(size.YValue / scaler); // Only need to be approximate

    starPos.XValue = (
        physModel().pos().XValue +
        (rand() % xMax) - xMax / 2
    );
    starPos.YValue = (
        physModel().pos().YValue +
        (rand() % yMax) - yMax / 2
    );
}

/* Game Object
-------------------------------------------------- */

StarField::StarField(StarFieldSpec::UPtr spec) :
    HasPhysOf(NewtonianPhysModel::UPtr(new NewtonianPhysModel(spec->pos, Vector2D(0, 0), Vector2D(0, 0), 0.0f))),
    HasGraphicsOf(StarFieldGraphicsModel::create(spec->size, spec->density, spec->image))
{
    // Thread dependencies
    trackPhysObserver(graphicsModelWPtr());
}

const ObjectFactory StarField::factory = [](ObjectSpec::UPtr spec) {
    return GameObject::Ptr(new StarField(static_unique_pointer_cast<StarFieldSpec>(move(spec))));
};

void StarField::setNumLayers(const unsigned int numLayers) {
    graphicsModel().setNumLayers(numLayers);
}

void StarField::setField(const Vector2D& size, const float density) {
    graphicsModel().setField(size, density);
}

void StarField::setSize(Vector2D size) {
    graphicsModel().setSize(size);
}

void StarField::setDensity(float density) {
    graphicsModel().setDensity(density);
}
