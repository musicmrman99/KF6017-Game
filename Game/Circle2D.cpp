#include "Circle2D.h"

#include <typeinfo>

#include "Point2D.h"
#include "Segment2D.h"
#include "Rectangle2D.h"
#include "AngledRectangle2D.h"

Circle2D::Circle2D() : mCentre(Vector2D(0, 0)), mdRadius(0) {}
Circle2D::Circle2D(float radius) : mCentre(Vector2D(0, 0)) {
    if (radius >= 0)
        this->mdRadius = radius;
    else
        this->mdRadius = -radius;
}
Circle2D::Circle2D(const Vector2D& centre, float radius) : Circle2D(radius) {
    this->mCentre = centre;
}

/* Specific Getters / Setters
-------------------- */

Vector2D Circle2D::GetCentre() const { return mCentre; }
float Circle2D::GetRadius() const { return mdRadius; }
float Circle2D::GetArea() const {
    return 3.141593f * mdRadius * mdRadius;
}

// ----------

void Circle2D::PlaceAt(const Vector2D& centre, float radius) {
    this->mCentre = centre;
    if (radius >= 0)
        this->mdRadius = radius;
    else
        this->mdRadius = -radius;
}

/* Implement IShape2D
-------------------- */

void Circle2D::emplace(const Vector2D& pos, const Vector2D& rot) {
    mCentre = pos;
}

// ----------

bool Circle2D::intersects(const IShape2D& other) const {
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

    // Undefined shape
    return false;
}

bool Circle2D::intersects(const Point2D& other) const {
    return((other.GetPosition() - mCentre).magnitude() < mdRadius);
}

bool Circle2D::intersects(const Segment2D& other) const {
    return other.intersects(*this);
}

bool Circle2D::intersects(const Circle2D& other) const {
    return((other.GetCentre() - mCentre).magnitude() < (mdRadius + other.GetRadius()));
}

bool Circle2D::intersects(const Rectangle2D& other) const {
    Point2D c(this->mCentre);
    if (c.Distance(other) < this->mdRadius)
        return true;
    else
        return false;
}

bool Circle2D::intersects(const AngledRectangle2D& other) const {
    return other.intersects(*this);
}

/* Other Utils
-------------------- */

float Circle2D::Distance(const Point2D& other) const {
    return((other.GetPosition() - mCentre).magnitude() - mdRadius);
}

float Circle2D::Distance(const Segment2D& other) const {
    return Distance(other.Intersection(mCentre));
}

float Circle2D::Distance(const Circle2D& other) const {
    return((other.GetCentre() - mCentre).magnitude() - (mdRadius + other.GetRadius()));
}

float Circle2D::Distance(const Rectangle2D& other) const {
    Point2D c(this->mCentre);
    return(c.Distance(other) - mdRadius);
}

// ----------

Vector2D Circle2D::Intersection(const Point2D& other) const {
    Vector2D toPoint = other.mPosition - mCentre;
    toPoint = toPoint.unitVector();
    return mCentre + toPoint * mdRadius;
}

Vector2D Circle2D::Intersection(const Segment2D& other) const {
    return Intersection(other.Intersection(*this));    // Implicit cast to Point2D
}

Vector2D Circle2D::Intersection(const Circle2D& other) const {
    return Intersection(Point2D(other.mCentre));
}

Vector2D Circle2D::Intersection(const Rectangle2D& other) const {
    Vector2D otherEdge = other.Intersection(*this) - mCentre;
    return (mCentre + otherEdge.unitVector() * mdRadius);
}

// ----------

Vector2D Circle2D::CollisionNormal(const Point2D& other) const {
    return (other.mPosition - mCentre).unitVector();
}

Vector2D Circle2D::CollisionNormal(const Segment2D& other) const {
    return (other.Intersection(*this) - mCentre).unitVector();
}

Vector2D Circle2D::CollisionNormal(const Circle2D& other) const {
    return (other.mCentre - mCentre).unitVector();
}

Vector2D Circle2D::CollisionNormal(const Rectangle2D& other) const {
    return (other.Intersection(*this) - mCentre).unitVector();
}
