#pragma once

#include "IShape2D.h"

#include "Segment2D.h"
#include "Circle2D.h"
#include "Rectangle2D.h"

struct AngledRectangle2DSides {
    Segment2D top;
    Segment2D left;
    Segment2D bottom;
    Segment2D right;
};

// A shape to manage a rectangle, with a centre, height and width, 
// rotated to any angle.
class AngledRectangle2D final : public IShape2D {
    friend Point2D;
    friend Segment2D;
    friend Circle2D;
    friend Rectangle2D;

private:
    float mHeight;             // Vertical height of the rectangle
    float mWidth;              // Horizontal width of the rectangle
    Vector2D mCentre;          // Centre of the rectangle
    float mAngle;              // Current angle of the rectangle in radians
    Circle2D mTrivialRejector; // Circle2D that fully encloses the rectangle
    Rectangle2D mLocalRectangle;

    void UpdateTrivialRejector();
    Vector2D TransformToLocal(Vector2D v) const;
    Vector2D TranformBackFromLocal(Vector2D v) const;

public:
    // Constructs an AngledRectangle2D at rotation 0
    // with a height and width of 0 at position (0, 0).
    AngledRectangle2D();

    // Constructs an AngledRectangle2D at rotation 0
    // with the given height and width at position (0, 0).
    AngledRectangle2D(float height, float width);

    // Constructs an AngledRectangle2D at rotation 0
    // with the given height, width and centre.
    AngledRectangle2D(Vector2D centre, float height, float width);

    /* Specific Getters / Setters
    -------------------- */

    // Returns the current angle
    float GetAngle() const;

    // Returns the current centre
    Vector2D GetCentre() const;

    // Returns the current height
    float GetHeight() const;

    // Returns the current width
    float GetWidth() const;

    AngledRectangle2DSides GetSides() const;

    // ----------

    // Sets the centre of the angled rectangle
    void SetCentre(const Vector2D& centre);

    // Sets the height and width of the angled rectangle
    void SetDimensions(float height, float width);

    // Sets the angle of the retangle
    void SetAngle(float angle);

    /* Implement IShape2D
    -------------------- */

    // Places the shape centred at the given position with the given rotation
    virtual void emplace(const Vector2D& pos, const Vector2D& rot) override;

    // ----------

    // Returns true if the AngledRectangle intersects with other shapes
    virtual bool intersects(const IShape2D& other) const override;

    bool intersects(const Point2D& other) const;
    bool intersects(const Circle2D& other) const;
    bool intersects(const Segment2D& other) const;
    bool intersects(const Rectangle2D& other) const;
    bool intersects(const AngledRectangle2D& other) const;
};
