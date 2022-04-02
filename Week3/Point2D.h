#pragma once

#include "IShape2D.h"

// Class to manage a 2D point shape
class Point2D final : public IShape2D {
    friend Segment2D;
    friend Circle2D;
    friend Rectangle2D;
    friend AngledRectangle2D;

private:
    Vector2D mPosition; // The position of the point

public:
    Point2D();                     // Sets position to (0, 0)
    Point2D(const Vector2D& copy); // Copy ctor
    Point2D(float x, float y);     // Designed for screen geometry, so y is inverted

    /* Specific Getters / Setters
    -------------------- */

    // Returns the vector location of the point
    Vector2D GetPosition() const;

    /* Implement IShape2D
    -------------------- */

    // Places the shape centred at the given position with the given rotation
    virtual void emplace(const Vector2D& pos, const Vector2D& rot) override;

    // ----------

    // Returns true if the point intersects the specified shape
    virtual bool intersects(const IShape2D& other) const override;

    // Overloads for specific shapes:

    // Returns true if the other is identical to this, false otherwise.
    bool intersects(const Point2D& other) const;

    // Returns true if this point lies on the 
    // Segment2D other.
    // False otherwise
    bool intersects(const Segment2D& other) const;

    // Returns true if the point lies within the circle
    // False otherwise
    bool intersects(const Circle2D& other) const;

    // Returns true if the point is within the rectangle
    // False otherwise
    // Note that Rectangle2D include the left and top edge,
    // but excludes the bottom and right edge
    bool intersects(const Rectangle2D& other) const;

    // Returns true if the point is within the angled rectangle
    // False otherwise
    bool intersects(const AngledRectangle2D& other) const;

    /* Other Utils
    -------------------- */

    Vector2D Intersection(const Point2D& other) const;     // Just returns the location of this Point2D
    Vector2D Intersection(const Segment2D& other) const;   // Returns the location of this point
    Vector2D Intersection(const Circle2D& other) const;    // Returns the location of this point
    Vector2D Intersection(const Rectangle2D& other) const; // Returns the location of this poin

    // ----------

    // Returns the distance between other and this
    float Distance(const Point2D& other) const;

    // Returns the distance from this point to the nearest 
    // point on the segment
    float Distance(const Segment2D& other) const;

    // Returns the distance from the point2D
    // to the edge of the circle
    // Note this will return a negative value if
    // this point is inside the circle (negative
    // of the distance to the edge).
    float Distance(const Circle2D& other) const;

    // Returns the distance from this point to the closest 
    // point on the rectangle.
    // Returns a negative number if the point is within the rectangle
    // (negative of the distance to the nearest edge)
    float Distance(const Rectangle2D& other) const;
};
