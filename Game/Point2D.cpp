#include "Point2D.h"

#include <typeinfo>

#include "Segment2D.h"
#include "Circle2D.h"
#include "Rectangle2D.h"
#include "AngledRectangle2D.h"

Point2D::Point2D() : mPosition(Vector2D(0, 0)) {}
Point2D::Point2D(float x, float y) : mPosition(Vector2D(x, y)) {}
Point2D::Point2D(const Vector2D& copy) : mPosition(copy) {}

/* Specific Getters / Setters
-------------------- */

Vector2D Point2D::GetPosition() const {
    return mPosition;
}

/* Implement IShape2D
-------------------- */

// Set the position to the given position. Ignores rotation
// (rotating a point makes no difference).
void Point2D::emplace(const Vector2D& pos, const Vector2D& rot) {
    mPosition = pos;
}

// ----------

bool Point2D::intersects(const IShape2D& other) const {
    // Is it a point?
    if (typeid(other) == typeid(Point2D))
        return static_cast<const Point2D*> (&other)->intersects(*this);

    // Is it a segment?
    if (typeid(other) == typeid(Segment2D))
        return static_cast<const Segment2D*> (&other)->intersects(*this);

    // Is it a circle?
    if (typeid(other) == typeid(Circle2D))
        return static_cast<const Circle2D*> (&other)->intersects(*this);

    // Is it a rectangle?
    if (typeid(other) == typeid(Rectangle2D))
        return static_cast<const Rectangle2D*> (&other)->intersects(*this);

    // Is it an angled rectangle?
    if (typeid(other) == typeid(AngledRectangle2D))
        return static_cast<const AngledRectangle2D*> (&other)->intersects(*this);

    // Assume it doesn't intersect an unknown shape
    return false;
}

bool Point2D::intersects(const Point2D& other) const {
    return(this->mPosition == other.mPosition);
}

bool Point2D::intersects(const Segment2D& other) const {
    double t1 = other.GetTFromX(this->mPosition.XValue);
    double t2 = other.GetTFromY(this->mPosition.YValue);
    if (t2 == t1) { // On LINE
        return (t2 >= 0 && t2 < 1.0); // Within ends of segment
    } else
        return false;
}

bool Point2D::intersects(const Circle2D& other) const {
    return ((mPosition - other.GetCentre()).magnitude() < other.GetRadius());
}

bool Point2D::intersects(const Rectangle2D& other) const {
    if (mPosition.XValue < other.GetCorner1().XValue
        || mPosition.XValue > other.GetCorner2().XValue
        || mPosition.YValue < other.GetCorner1().YValue
        || mPosition.YValue > other.GetCorner2().YValue)
        return false;
    else
        return true;
}

bool Point2D::intersects(const AngledRectangle2D& other) const {
    return other.intersects(*this);
}

/* Other Utils
-------------------- */

float Point2D::Distance(const Point2D& other) const {
    return (this->mPosition - other.mPosition).magnitude();
}

float Point2D::Distance(const Segment2D& other) const {
    // Project the point onto the line and find the parameter, t
    // using parametric equation.
    // Hmm. Should be ((C - S) . ( E - S ) ) / L^2

    return other.Distance(*this);
}

float Point2D::Distance(const Circle2D& other) const {
    return ((mPosition - other.GetCentre()).magnitude() - other.GetRadius());
}

float Point2D::Distance(const Rectangle2D& other) const {
    // 9 possibilities
    if (mPosition.XValue < other.GetCorner1().XValue) {              // Left side
        if (mPosition.YValue < other.GetCorner1().YValue) {            // Top left
            return (mPosition - other.GetCorner1()).magnitude();

        } else if (mPosition.YValue >= other.GetCorner2().YValue) {    // Bottom left
            Vector2D bl(other.GetCorner1().XValue, other.GetCorner2().YValue);
            return (mPosition - bl).magnitude();

        } else {                                                       // Middle left
            return other.GetCorner1().XValue - mPosition.XValue;
        }

    } else if (mPosition.XValue >= other.GetCorner2().XValue) {      // Right side
        if (mPosition.YValue < other.GetCorner1().YValue) {            // Top right
            Vector2D tr(other.GetCorner2().XValue, other.GetCorner1().YValue);
            return (mPosition - tr).magnitude();

        } else if (mPosition.YValue >= other.GetCorner2().YValue) {    // Bottom right
            return (mPosition - other.GetCorner2()).magnitude();

        } else {                                                       // Middle right
            return mPosition.XValue - other.GetCorner2().XValue;
        }

    } else {                                                         // Centre line
        if (mPosition.YValue < other.GetCorner1().YValue) {            // Top 
            return other.GetCorner1().YValue - mPosition.YValue;

        } else if (mPosition.YValue >= other.GetCorner2().YValue) {    // Bottom
            return mPosition.YValue - other.GetCorner2().YValue;

        } else {                                                       // Inside rectangle - get distances to edges
            float t, b, l, r;
            t = mPosition.YValue - other.GetCorner1().YValue;
            b = other.GetCorner2().YValue - mPosition.YValue;
            l = mPosition.XValue - other.GetCorner1().XValue;
            r = other.GetCorner2().XValue - mPosition.XValue;

            float smallest = t;
            if (smallest > b)
                smallest = b;
            if (smallest > l)
                smallest = l;
            if (smallest > r)
                smallest = r;

            return -smallest; // Negative, as inside the rectangle
        }
    }
}

// ----------

// Uhh... Closest point on a point IS the point.

Vector2D Point2D::Intersection(const Rectangle2D& other) const { return mPosition; }
Vector2D Point2D::Intersection(const Point2D& other)     const { return mPosition; }
Vector2D Point2D::Intersection(const Segment2D& other)   const { return mPosition; }
Vector2D Point2D::Intersection(const Circle2D& other)    const { return mPosition; }
