#include "AngledRectangle2D.h"

#include <typeinfo>
#include <cmath>

#include "Point2D.h"
#include "Segment2D.h"

AngledRectangle2D::AngledRectangle2D()
    : mWidth(0), mHeight(0), mAngle(0) {
    UpdateTrivialRejector();
}

AngledRectangle2D::AngledRectangle2D(float height, float width)
    : mCentre(Vector2D(0, 0)), mWidth(width), mHeight(height) {
    UpdateTrivialRejector();
    mLocalRectangle.PlaceAt(mHeight / 2, -mWidth / 2, -mHeight / 2, mWidth / 2);
}

// Constructs an AngledRectangle2D at rotation 0, with the given height
// and width and centre
AngledRectangle2D::AngledRectangle2D(Vector2D centre, float height, float width)
    : mCentre(centre), mWidth(width), mHeight(height) {
    UpdateTrivialRejector();
    mLocalRectangle.PlaceAt(mHeight / 2, -mWidth / 2, -mHeight / 2, mWidth / 2);
}

/* Specific Getters / Setters
-------------------- */

// Returns the current angle
float AngledRectangle2D::GetAngle() const {
    return mAngle;
}

// Returns the current centre
Vector2D AngledRectangle2D::GetCentre() const {
    return mCentre;
}

// Returns the current height
float AngledRectangle2D::GetHeight() const {
    return mHeight;
}

// Returns the current width
float AngledRectangle2D::GetWidth() const {
    return mHeight;
}

// Get a structure containing the top, left, bottom, and right
// sides as segments.
AngledRectangle2DSides AngledRectangle2D::GetSides() const {
    // Uses the same algorithm as intersects(Rectangle2D&)
    // and intersects(AngledRectangle2D&).
    AngledRectangle2DSides sides;

    // Get corners
    Vector2D p1(mWidth / 2, mHeight / 2);
    p1 = p1.rotatedBy(mAngle);
    Vector2D p2(mWidth / 2, -mHeight / 2);
    p2 = p2.rotatedBy(mAngle);
    Vector2D p3 = -p1;
    Vector2D p4 = -p2;

    // Get sides
    sides.top.PlaceAt(mCentre + p1, mCentre + p2);
    sides.left.PlaceAt(mCentre + p2, mCentre + p3);
    sides.bottom.PlaceAt(mCentre + p3, mCentre + p4);
    sides.right.PlaceAt(mCentre + p4, mCentre + p1);

    return sides;
}

// ----------

// Sets the centre of the angled rectangle
void AngledRectangle2D::SetCentre(const Vector2D& centre) {
    mCentre = centre;
    UpdateTrivialRejector();
}

// Sets the height and width of the angled rectangle
void AngledRectangle2D::SetDimensions(float height, float width) {
    mWidth = width;
    mHeight = height;
    UpdateTrivialRejector();
    mLocalRectangle.PlaceAt(mHeight / 2, -mWidth / 2, -mHeight / 2, mWidth / 2);
}

// Sets the angle of the rectangle
void AngledRectangle2D::SetAngle(float angle) {
    mAngle = angle;
}

/* Implement IShape2D
-------------------- */

void AngledRectangle2D::emplace(const Vector2D& pos, const Vector2D& rot) {
    SetCentre(pos);
    SetAngle(rot.angle());
}

// Returns true if the AngledRectangle intersects with other shapes
bool AngledRectangle2D::intersects(const IShape2D& other) const {
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

bool AngledRectangle2D::intersects(const Point2D& other) const {
    if (!other.intersects(mTrivialRejector)) {
        return false;
    } else {
        Vector2D p = TransformToLocal(other.GetPosition());
        return mLocalRectangle.intersects(p);
    }
}

bool AngledRectangle2D::intersects(const Segment2D& other) const {
    if (!other.intersects(mTrivialRejector)) {
        return false;
    } else {
        Segment2D s;
        s.PlaceAt(TransformToLocal(other.mStart), TransformToLocal(other.mEnd));
        return mLocalRectangle.intersects(s);
    }
}

bool AngledRectangle2D::intersects(const Circle2D& other) const {
    if (!other.intersects(mTrivialRejector)) {
        return false;
    } else {
        Circle2D c(TransformToLocal(other.mCentre), other.mdRadius);
        return mLocalRectangle.intersects(c);
    }
}

bool AngledRectangle2D::intersects(const Rectangle2D& other) const {
    if (!other.intersects(mTrivialRejector)) {
        return false;
    } else {
        // Do your corners intersect it?
        Vector2D p1(mWidth / 2, mHeight / 2);
        p1 = p1.rotatedBy(mAngle);
        if (other.intersects(mCentre + p1))
            return true;
        Vector2D p3 = -p1;
        if (other.intersects(mCentre + p3))
            return true;
        Vector2D p2(mWidth / 2, -mHeight / 2);
        p2 = p2.rotatedBy(mAngle);
        if (other.intersects(mCentre + p2))
            return true;
        Vector2D p4 = -p2;
        if (other.intersects(mCentre + p4))
            return true;

        // Do your segments intersect it?
        Segment2D s;
        s.PlaceAt(p1 + mCentre, p2 + mCentre);
        if (other.intersects(s))
            return true;
        s.PlaceAt(p2 + mCentre, p3 + mCentre);
        if (other.intersects(s))
            return true;
        s.PlaceAt(p3 + mCentre, p4 + mCentre);
        if (other.intersects(s))
            return true;
        s.PlaceAt(p4 + mCentre, p1 + mCentre);
        if (other.intersects(s))
            return true;
        return false;
    }
}

bool AngledRectangle2D::intersects(const AngledRectangle2D& other) const {
    if (!other.intersects(mTrivialRejector)) {
        return false;
    } else {
        // Do your corners intersect it?
        Vector2D p1(mWidth / 2, mHeight / 2);
        p1 = p1.rotatedBy(mAngle);
        if (other.intersects(mCentre + p1))
            return true;
        Vector2D p3 = -p1;
        if (other.intersects(mCentre + p3))
            return true;
        Vector2D p2(mWidth / 2, -mHeight / 2);
        p2 = p2.rotatedBy(mAngle);
        if (other.intersects(mCentre + p2))
            return true;
        Vector2D p4 = -p2;
        if (other.intersects(mCentre + p4))
            return true;

        // Do your segments intersect it?
        Segment2D s;
        s.PlaceAt(p1 + mCentre, p2 + mCentre);
        if (other.intersects(s))
            return true;
        s.PlaceAt(p2 + mCentre, p3 + mCentre);
        if (other.intersects(s))
            return true;
        s.PlaceAt(p3 + mCentre, p4 + mCentre);
        if (other.intersects(s))
            return true;
        s.PlaceAt(p4 + mCentre, p1 + mCentre);
        if (other.intersects(s))
            return true;

        return false;
    }
}

/* Private Utils
-------------------- */

void AngledRectangle2D::UpdateTrivialRejector() {
    mTrivialRejector.PlaceAt(mCentre, std::sqrt(mWidth * mWidth / 4 + mHeight * mHeight / 4));
}

Vector2D AngledRectangle2D::TransformToLocal(Vector2D v) const {
    v = v - mCentre;
    v = v.rotatedBy(-mAngle);
    return v;
}

Vector2D AngledRectangle2D::TranformBackFromLocal(Vector2D v) const {
    v = v.rotatedBy(mAngle);
    v = v + mCentre;
    return v;
}
