#pragma once

#include "IShape2D.h"

// A segment is a line with a start and an end
// I.e. not infinite length
class Segment2D final : public IShape2D {
    friend Point2D;
    friend Circle2D;
    friend Rectangle2D;
    friend AngledRectangle2D;

private:
    Vector2D mStart;
    Vector2D mEnd;

public:
    Segment2D(); // Creates zero-length segment at position (0, 0)

    /* Specific Getters / Setters
    -------------------- */

    // Returns the position of the start of the segment
    Vector2D GetStart() const;

    // Returns the position of the end of the segment
    Vector2D GetEnd() const;

    // Returns the length of the segment
    float GetLength() const;

    /* Returns t, treating the line as a parametric equation.
    // Can return a t below 0 or above 1 if x is beyond the ends of the segment
    // If the line is horizontal, returns 1.0 if x is on the line, or
     returns INT_MAX otherwise*/
    float GetTFromX(float x) const;

    // Returns t, treating the line as a parametric equation.
    // Can return a t below 0 or above 1 if y is beyond the ends of the segment
    // If the line is vertical, returns 1.0 if y is on the line, or
    // returns INT_MAX otherwise
    float GetTFromY(float y) const;

    // Returns the vector of a point on the line defined by t,
    // using parametric equations
    // x=x1 + t(x2-x1)
    // y=y1 + t(y2-y1)
    Vector2D PointFromT(float t) const;

    // ----------

    // Places segment in position defined by the start and end.
    // Start is part of the line, end isn't.
    void PlaceAt(const Vector2D& start, const Vector2D& end);

    // Sets the start to the point on the line defined by t,
    // using parametric equations
    // x=x1 + t(x2-x1)
    // y=y1 + t(y2-y1)
    // Note, if t is less than 0, this will lengthen the line.
    // If t is greater than 1.0 this will reverse the direction of the line
    void SetStartTo(float t);

    // Sets the end to the point on the line defined by t,
    // using parametric equations
    // x=x1 + t(x2-x1)
    // y=y1 + t(y2-y1)
    // Note, if t is greater than 1.0, this will lengthen the line.
    // If t is less than 0.0 this will reverse the direction of the line
    void SetEndTo(float t);

    // Clips the segment to the t-values provided
    void ClipTo(float minT, float maxT);

    /* Implement IShape2D
    -------------------- */

    // Places the shape centred at the given position with the given rotation
    virtual void emplace(const Vector2D& pos, const Vector2D& rot) override;

    // ----------

    // Returns true if the segment intersects the specified shape
    virtual bool intersects(const IShape2D& other) const override;

    // Overloads for specific shapes:

    // Returns true if other lies on the segment
    // False otherwise
    // Note that the point is not on the segment if it is beyond
    // the start or end
    bool intersects(const Point2D& other) const;

    // Returns true if the two segments intersect
    // Returns false otherwise
    bool intersects(const Segment2D& other) const;

    // Returns true if the Segment intersect the circle
    // False otherwise
    bool intersects(const Circle2D& other) const;

    // Returns true if the segment intersects the rectangle
    // False otherwise
    bool intersects(const Rectangle2D& other) const;

    // Returns true if the segment intersects the angled rectangle
    // False otherwise
    bool intersects(const AngledRectangle2D& other) const;

    /* Other Utils
    -------------------- */

    // Returns the closest point on the segment
    // to other
    Vector2D Intersection(const Point2D& other) const;

    // Returns the closest point on this segment to the
    // other segments (point of intersection if they intersect)
    Vector2D Intersection(const Segment2D& other) const;

    // Returns the closest point on the segment to the circle.
    // if the segement intersects the circle, this returns the
    // deepest point on the segment
    Vector2D Intersection(const Circle2D& other) const;

    // Returns the closest point on the Segment to the rectangle
    // If they intersect, returns the mid-point of the part of
    // the segment that intersects
    Vector2D Intersection(const Rectangle2D& other) const;

    // ----------

    // Returns the point of intersection with a rectangle.
    // If there are two points, returns the once closest to the start.
    // If no intersection is found, returns the end of the segment
    Vector2D FirstIntersection(const Rectangle2D& other) const;

    // ----------

    // Returns true if the segments are parallel (in either direction)
    // Allows for founding errors with a 0.01% tolerance
    bool ParallelTo(const Segment2D& other) const;

    // Returns true if the segment is parallel to a vector(in either direction)
    // Allows for founding errors with a 0.01% tolerance
    bool ParallelTo(const Vector2D& other) const;

    // ----------

    // Distance from other to the closest point
    // on the segment
    float Distance(const Point2D& other) const;

    // Returns the closest distance between the two segments
    float Distance(const Segment2D& other) const;

    // Returns the closest distance from the segment to the circle
    // If the segment intersects the circle, this
    // returns -ve the distance from the deepest point on the
    // segment to the edge of the circle
    float Distance(const Circle2D& other) const;

    // Returns the distance from the Segment to the Rectangle
    // If they intersect, returns a negative number
    // Slow function
    float Distance(const Rectangle2D& other) const;
};
