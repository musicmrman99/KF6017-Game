#pragma once

#include "IShape2D.h"

// Class to manage a 2D circle
class Circle2D final : public IShape2D {
    friend Point2D;
    friend Segment2D;
    friend Rectangle2D;
    friend AngledRectangle2D;

private:
    Vector2D mCentre; // Position of the centre
    float mdRadius;   // Radius

public:
    Circle2D(); // Creates a zero-size (radius) circle at (0, 0)

    // Places the Circle2D with specified radius at (0, 0).
    // A negative radius is converted to positive.
    Circle2D(float radius);

    // Places the Circle2D at the specified location with specified radius.
    // A negative radius is converted to positive.
    Circle2D(const Vector2D& centre, float radius);

    /* Specific Getters / Setters
    -------------------- */

    // Returns the location of the centre of the circle
    Vector2D GetCentre() const;

    // Returns the radius of the circle
    float GetRadius() const;

    // Returns the area of the circle
    float GetArea() const;

    // ----------

    // Places the Circle2D at the specified location with specified radius.
    // A negative radius is converted to positive
    void PlaceAt(const Vector2D& centre, float radius);

    /* Implement IShape2D
    -------------------- */

    // Places the shape centred at the given position with the given rotation
    virtual void emplace(const Vector2D& pos, const Vector2D& rot) override;

    // ----------

    // Returns true if the circle intersects the specified shape
    virtual bool intersects(const IShape2D& other) const override;

    // Overloads for specific shapes:

    // Returns true if the Point is inside the circle
    // False otherwise
    bool intersects(const Point2D& other) const;

    // Returns true if the segment intersects the circle
    // False otherwise
    bool intersects(const Segment2D& other) const;

    // Returns true if the two circles intersect
    // false otherwise
    bool intersects(const Circle2D& other) const;

    // Returns true if the circle intersects the rectangle
    // false otherwise
    bool intersects(const Rectangle2D& other) const;

    // Returns true if the circle intersects the angled rectangle
    // false otherwise
    bool intersects(const AngledRectangle2D& other) const;

    /* Other Utils
    -------------------- */

    // Returns the point on the edge of the circle closest
    // to the point (whether inside or outside)
    // Or returns the centre of this circle if the point
    // is exactly equal to the circle's centre
    Vector2D Intersection(const Point2D& other) const;

    // If the segment intersects the  circle,
    // Returns the point on the surface of the circle
    // closest to the mid-point of the intersecting part of the segment.
    // otherwise returns the closest point on the edge of the 
    // circle to the segment
    Vector2D Intersection(const Segment2D& other) const;

    // Returns the closest point on the edge of this circle
    // to the centre of the other circle. (Or the centre of
    // this circle if the two centres are equal)
    Vector2D Intersection(const Circle2D& other) const;

    // Returns the point on the surface of the circle
    // closest to the intersection (or the nearest point on
    // the rectangle if no intersection
    Vector2D Intersection(const Rectangle2D& other) const;

    // ----------

    // Returns the distance from the edge of the circle
    // to the rectangle.
    // Returns a negative number if the circle and
    // the rectangle overlap
    float Distance(const Rectangle2D& other) const;

    // Returns the distance from the point to the circle
    // If the point is inside the circle,
    // returns -ve the distance from the point to
    // the edge of the circle
    float Distance(const Point2D& other) const;

    // Returns the closest distance from the circle
    // to the segment
    float Distance(const Segment2D& other) const;

    // returns the distance between the two circles
    // This will be negative if the circles intersect
    float Distance(const Circle2D& other) const;

    // ----------

    // Returns the normal to the (presumed)
    // collision between the other point and
    // this circle
    Vector2D CollisionNormal(const Point2D& other) const;

    // Returns the normal to the (presumed)
    // collision between the other segment and
    // this circle
    Vector2D CollisionNormal(const Segment2D& other) const;

    // Returns the normal to the (presumed)
    // collision between the other circle and
    // this circle
    Vector2D CollisionNormal(const Circle2D& other) const;

    // Returns the normal to the (presumed)
    // collision between the other rectangle and
    // this circle
    Vector2D CollisionNormal(const Rectangle2D& other) const;
};
