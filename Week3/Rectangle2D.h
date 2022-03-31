#pragma once

#include "IShape2D.h"

// Class to manage a 2D rectangle shape.
// Shape must be orthogonally-aligned
class Rectangle2D final : public IShape2D {
    friend Point2D;
    friend Segment2D;
    friend Circle2D;
    friend AngledRectangle2D;

private:
    Vector2D mCorner1; // Bottom left assuming regular cartesian
    Vector2D mCorner2; // Top right assuming regular cartesian

public:
    Rectangle2D(); // Sets all corners to (0, 0)

    /* Specific Getters / Setters
    -------------------- */

    // Returns the centre of the rectangle
    Vector2D GetCentre() const;

    // Returns the location of the top left corner
    Vector2D GetTopLeft() const;

    // Returns the location of the bottom right corner
    Vector2D GetBottomRight() const;

    // Returns the location of the bottom left corner
    Vector2D GetBottomLeft() const;

    // Returns the location of the top right corner
    Vector2D GetTopRight() const;

    // Returns the location of the top left corner
    Vector2D GetCorner1() const;

    // Returns the location of the bottom right corner
    Vector2D GetCorner2() const;

    // Returns the area of the rectangle
    float GetArea() const;

    // ----------

    // Places the rectangle at the specified coordinates
    // the topRight should have larger X and Y values
    // than the bottomLeft.
    // If this is not right, x and y values are swapped by
    // this function as appropriate
    // The rectangle includes topLeft, but not bottomRight
    void PlaceAt(Vector2D bottomLeft, Vector2D topRight);

    // Places the rectangle at the specified location.
    // if top > bottom, these values are reversed
    // if left > right, these values are reversed
    // The rectangle includes the top and left edges,
    // but not the bottom or right
    void PlaceAt(float top, float left, float bottom, float right);

    /* Implement IShape2D
    -------------------- */

    // Places the shape centred at the given position with the given rotation
    virtual void emplace(const Vector2D& pos, const Vector2D& rot) override;

    // ----------

    // Returns true if the rectangle intersects the specified shape
    virtual bool intersects(const IShape2D& other) const override;

    // Returns true if the point intersects the rectangle
    // false otherwise
    bool intersects(const Point2D& other) const;

    // Returns true if the segment intersects the
    // rectangle. False otherwise
    bool intersects(const Segment2D& other) const;

    // Returns true if the circle intersects the 
    // rectangle.
    // False otherwise
    bool intersects(const Circle2D& other) const;

    // Returns true if the rectangles intersect
    // false otherwise
    bool intersects(const Rectangle2D& other) const;

    // Returns true if the rectangles intersect
    // false otherwise
    bool intersects(const AngledRectangle2D& other) const;

    /* Other Utils
    -------------------- */

    // Returns the point on the surface of the rectangle closest to the
    // point
    Vector2D Intersection(const Point2D& other) const;

    // Returns the point on the edge of the rectangle
    // closest to segment (or the deepest part of the
    // segment if the segment intersects the rectangle)
    Vector2D Intersection(const Segment2D& other) const;

    // Returns the point on the edge of the rectangle
    // where intersection occured (or the closest
    // point to the circle if no intersection)
    Vector2D Intersection(const Circle2D& other) const;

    // Returns the point on this rectangle that is closest
    // to the point of intersection with the other rectangle.
    // If the two rectangles do not overlap, returns the
    // point on this rectangle that is closest to the other
    // rectangle
    Vector2D Intersection(const Rectangle2D& other) const;

    // ----------

    // Returns the distance from the point to the rectangle
    // If the point is inside the rectangle,
    // this returns -ve the distance to the closest edge
    float Distance(const Point2D& other) const;

    // Returns the closest distance from the segment
    // to the rectangle. Returns a -ve value if the segment intersects
    // the rectangle
    float Distance(const Segment2D& other) const;

    // Returns the shortest distance from the circle 
    // to the rectangle.
    // Returns a negative number if the circle
    // intersects the rectangle
    float Distance(const Circle2D& other) const;

    // Returns the distance between the two closest points 
    // on the two rectangles. Returns zero if the rectangles
    // overlap
    float Distance(const Rectangle2D& other) const;

    // ----------

    // Returns the normal to the surface of the rectangle closest
    // to the point of collision.
    // (1,0)  (-1,0)  (0,1)  or (0,-1)
    // If no collision, returns (0,0)
    Vector2D CollisionNormal(const Point2D& other) const;

    // Returns the normal of the probable collision between
    // the rectangle and the circle.
    // (If no collision returns a unit vector from the
    // closest point on the edge of the rectangle to
    // the centre of the circle
    Vector2D CollisionNormal(const Circle2D& other) const;

    // Returns the normal of the probable collision between
    // this rectangle and the passed rectangle. The normal
    // points away from this Rectangle, either (0,1) (0,-1)
    // (1,0) or (-1,0) depending on which is the closest edge.
    // (If there is no collision, it returns a vector pointing 
    // away from the closest edge on this rectangle
    Vector2D CollisionNormal(const Rectangle2D& other) const;

    // Not implemented  *************************************
    Vector2D CollisionNormal(const Segment2D& other) const;

    // ----------

    // Returns the length of the supplied segment that passes through the rectangle
    // Returns zero if the segment does not intersect the rectangle
    float LengthThrough(const Segment2D& other) const;

    // Returns the supplied segment, clipped to fit inside the
    // rectangle.
    // Returns a zero length segment if the segment does not
    // intersect the rectangle
    Segment2D Clip(Segment2D other) const;
};
