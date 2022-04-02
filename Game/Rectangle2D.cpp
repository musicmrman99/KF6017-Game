#include "Rectangle2D.h"

#include <typeinfo>

#include "Point2D.h"
#include "Segment2D.h"
#include "Circle2D.h"
#include "AngledRectangle2D.h"

Rectangle2D::Rectangle2D() : mCorner1(Vector2D(0, 0)), mCorner2(Vector2D(0, 0)) {}

/* Specific Getters / Setters
-------------------- */

Vector2D Rectangle2D::GetCentre() const {
    return (mCorner1 + mCorner2) / 2;
}

Vector2D Rectangle2D::GetCorner1() const { return mCorner1; }
Vector2D Rectangle2D::GetCorner2() const { return mCorner2; }

Vector2D Rectangle2D::GetBottomLeft() const { return mCorner1; }
Vector2D Rectangle2D::GetTopRight() const { return mCorner2; }

Vector2D Rectangle2D::GetTopLeft() const {
    return Vector2D(mCorner1.XValue, mCorner2.YValue);
}
Vector2D Rectangle2D::GetBottomRight() const {
    return Vector2D(mCorner2.XValue, mCorner1.YValue);
}

float Rectangle2D::GetArea() const {
    float h = mCorner1.XValue - mCorner2.XValue;
    float w = mCorner1.YValue - mCorner2.YValue;

    float a = h * w;

    if (a < 0) return -a;
    else return a;
}

// ----------

void Rectangle2D::PlaceAt(Vector2D bottomLeft, Vector2D topRight) {
    // Verify corner 1 is bottom left and corner 2 is top right
    float top, left, bottom, right;
    if (bottomLeft.XValue <= topRight.XValue) {
        left = bottomLeft.XValue;
        right = topRight.XValue;
    } else {
        right = bottomLeft.XValue;
        left = topRight.XValue;
    }

    if (bottomLeft.YValue <= topRight.YValue) {
        bottom = bottomLeft.YValue;
        top = topRight.YValue;
    } else {
        top = bottomLeft.YValue;
        bottom = topRight.YValue;
    }

    mCorner1.set(left, bottom);
    mCorner2.set(right, top);
}

void Rectangle2D::PlaceAt(float top, float left, float bottom, float right) {
    PlaceAt(Vector2D(left, top), Vector2D(right, bottom));
}

/* Implement IShape2D
-------------------- */

void Rectangle2D::emplace(const Vector2D& pos, const Vector2D& rot) {
    Vector2D size = this->mCorner2 - this->mCorner1;
    PlaceAt(
        pos - size / 2,
        pos + size / 2
    );
}

// ----------

bool Rectangle2D::intersects(const IShape2D& other) const {
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

bool Rectangle2D::intersects(const Point2D& other) const {
    return other.intersects(*this);
}

bool Rectangle2D::intersects(const Segment2D& other) const {
    return other.intersects(*this);
}

bool Rectangle2D::intersects(const Circle2D& other) const {
    return other.intersects(*this);
}

bool Rectangle2D::intersects(const Rectangle2D& other) const {
    if (other.mCorner1.XValue >= this->mCorner2.XValue
        || other.mCorner2.XValue <= this->mCorner1.XValue
        || other.mCorner1.YValue >= this->mCorner2.YValue
        || other.mCorner2.YValue <= this->mCorner1.YValue)
        return false;
    else
        return true;
}

bool Rectangle2D::intersects(const AngledRectangle2D& other) const {
    return other.intersects(*this);
}

/* Other Utils
-------------------- */

Vector2D Rectangle2D::Intersection(const Point2D& other) const {
    // 9 possibilities
    if (other.mPosition.XValue < mCorner1.XValue) {    // Left side
        if (other.mPosition.YValue < mCorner1.YValue) {    // Top left
            return mCorner1;
        } else if (other.mPosition.YValue >= GetCorner2().YValue) {    // Bottom left
            return GetBottomLeft();
        } else {    // Middle left
            return Vector2D(mCorner1.XValue, other.mPosition.YValue);
        }

    } else if (other.mPosition.XValue >= mCorner2.XValue) {    // Right side
        if (other.mPosition.YValue < mCorner1.YValue) {    // Top right
            return GetTopRight();
        } else if (other.mPosition.YValue >= mCorner2.YValue) {    // Bottom right
            return mCorner2;
        } else {    // Middle right
            return Vector2D(mCorner2.XValue, other.mPosition.YValue);
        }
    } else {    // Centre line
        if (other.mPosition.YValue < mCorner1.YValue) {    // Top 
            return Vector2D(other.mPosition.XValue, mCorner1.YValue);
        } else if (other.mPosition.YValue >= mCorner2.YValue) {    // Bottom
            return Vector2D(other.mPosition.XValue, mCorner2.YValue);
        } else {    // Inside rectangle  get distances to edges
            double t, b, l, r;
            t = other.mPosition.YValue - mCorner1.YValue;
            b = mCorner2.YValue - other.mPosition.YValue;
            l = other.mPosition.XValue - mCorner1.XValue;
            r = mCorner2.XValue - other.mPosition.XValue;

            double smallest = t;
            if (smallest > b)
                smallest = b;
            if (smallest > l)
                smallest = l;
            if (smallest > r)
                smallest = r;

            if (smallest == t)
                return Vector2D(other.mPosition.XValue, mCorner1.YValue);
            else if (smallest == b)
                return Vector2D(other.mPosition.XValue, mCorner2.YValue);
            else if (smallest == l)
                return Vector2D(mCorner1.XValue, other.mPosition.YValue);
            else        // Smallest ==r
                return Vector2D(mCorner2.XValue, other.mPosition.YValue);
        }
    }

}

Vector2D Rectangle2D::Intersection(const Segment2D& other) const {
    // Get the intersection point on the other segment
    // and return the point closest to it
    return Intersection(other.Intersection(*this));
}

Vector2D Rectangle2D::Intersection(const Circle2D& other) const {
    // Return the point closest to the centre of the circle
    return Intersection(other.mCentre);
}

Vector2D Rectangle2D::Intersection(const Rectangle2D& other) const {
    Rectangle2D clipRect = other;        // A rectangle that describes the overlap
                                        // between the two rectangles
    // Clip the left side
    if (clipRect.mCorner1.XValue < this->mCorner1.XValue)
        clipRect.mCorner1.XValue = this->mCorner1.XValue;

    // Clip the right side
    if (clipRect.mCorner2.XValue > this->mCorner2.XValue)
        clipRect.mCorner2.XValue = this->mCorner2.XValue;

    // Clip the top side
    if (clipRect.mCorner1.YValue < this->mCorner1.YValue)
        clipRect.mCorner1.YValue = this->mCorner1.YValue;

    // Clip the bottom side
    if (clipRect.mCorner2.YValue > this->mCorner2.YValue)
        clipRect.mCorner2.YValue = this->mCorner2.YValue;

    // Note that clip rect may be "inverted" if there is no intersection
    // i.e. top > bottom or left>right
    // Not a problem.

    // Return the intersection point of the centre of the clipped rectangle

    return Intersection(clipRect.GetCentre());
}

// ----------

float Rectangle2D::Distance(const Point2D& other) const {

    return other.Distance(*this);
}

// TODO: Distance to a Segment2D

float Rectangle2D::Distance(const Circle2D& other) const {

    return other.Distance(*this);
}

float Rectangle2D::Distance(const Rectangle2D& other) const {
    Rectangle2D clipRect = other;        // A rectangle that describes the overlap
                                        // between the two rectangles
    // Clip the left side
    if (clipRect.mCorner1.XValue < this->mCorner1.XValue)
        clipRect.mCorner1.XValue = this->mCorner1.XValue;

    // Clip the right side
    if (clipRect.mCorner2.XValue > this->mCorner2.XValue)
        clipRect.mCorner2.XValue = this->mCorner2.XValue;

    // Clip the top side
    if (clipRect.mCorner1.YValue < this->mCorner1.YValue)
        clipRect.mCorner1.YValue = this->mCorner1.YValue;

    // Clip the bottom side
    if (clipRect.mCorner2.YValue > this->mCorner2.YValue)
        clipRect.mCorner2.YValue = this->mCorner2.YValue;

    // Check to see if the clip rect has been inverted horizontally
    if (clipRect.mCorner1.XValue < clipRect.mCorner2.XValue) {
        // If it HASN'T, flatten the rectangle horizontally
        clipRect.mCorner1.XValue = clipRect.mCorner2.XValue;
    }
    // Check to see if the clip rect has been inverted vertically
    if (clipRect.mCorner1.YValue < clipRect.mCorner2.YValue) {
        // If it HASN'T, flatten the rectangle vertically
        clipRect.mCorner1.YValue = clipRect.mCorner2.YValue;
    }

    // Return the diagonal of the resultant rectangle (this will be zero) if
    // there is overlap
    return (clipRect.mCorner1 - clipRect.mCorner2).magnitude();
}

// ----------

Vector2D Rectangle2D::CollisionNormal(const Point2D& other)const {
    Vector2D answer;
    double t, b, l, r;
    t = other.GetPosition().YValue - GetCorner1().YValue;
    b = GetCorner2().YValue - other.GetPosition().YValue;
    l = other.GetPosition().XValue - GetCorner1().XValue;
    r = GetCorner2().XValue - other.GetPosition().XValue;

    double smallest = t;
    {
        answer.set(0, 1);
    }
    if (smallest > b) {
        answer.set(0, -1);
        smallest = b;
    }
    if (smallest > l) {
        answer.set(-1, 0);
        smallest = l;
    }
    if (smallest > r) {
        answer.set(1, 0);
        smallest = r;
    }
    if (smallest < 0) {
        answer.set(0, 0);
    }
    return answer;
}

Vector2D Rectangle2D::CollisionNormal(const Segment2D& other) const {
    return CollisionNormal(other.Intersection(*this));
}

Vector2D Rectangle2D::CollisionNormal(const Circle2D& other) const {
    if (intersects(Vector2D(other.mCentre)))        // Centre of circle inside rectangle
    {
        float t, b, l, r;
        t = other.mCentre.YValue - mCorner1.YValue;
        b = mCorner2.YValue - other.mCentre.YValue;
        l = other.mCentre.XValue - mCorner1.XValue;
        r = mCorner2.XValue - other.mCentre.XValue;

        float smallest = t;
        if (smallest > b)
            smallest = b;
        if (smallest > l)
            smallest = l;
        if (smallest > r)
            smallest = r;

        if (smallest == t)
            return Vector2D(0, 1);
        else if (smallest == b)
            return Vector2D(0, -1);
        else if (smallest == l)
            return Vector2D(-1, 0);
        else        // Smallest ==r
            return Vector2D(1, 0);
    } else        // Centre of circle outside rectangle
    {
        Vector2D edge = Intersection(Point2D(other.mCentre));
        Vector2D normal = other.mCentre - edge;
        return normal.unitVector();
    }
}

Vector2D Rectangle2D::CollisionNormal(const Rectangle2D& other) const {
    Vector2D answer(0, 1);        // Assume an upwards collision
    // Get distance between this bottom and other top
    float distance = abs(this->mCorner1.YValue - other.mCorner2.YValue);

    // See if downwards collision is smaller
    if (distance > abs(this->mCorner2.YValue - other.mCorner1.YValue)) {
        answer.set(0, -1);
        distance = abs(this->mCorner2.YValue - other.mCorner1.YValue);
    }

    // See if leftwards collision is smaller
    if (distance > abs(this->mCorner1.XValue - other.mCorner2.XValue)) {
        answer.set(-1, 0);
        distance = abs(this->mCorner1.XValue - other.mCorner2.XValue);
    }

    // See if rightwards collision is smaller
    if (distance > abs(this->mCorner2.XValue - other.mCorner1.XValue)) {
        answer.set(1, 0);
        distance = abs(this->mCorner2.XValue - other.mCorner1.XValue);
    }

    return answer;
}

// ----------

float Rectangle2D::LengthThrough(const Segment2D& other) const {
    Segment2D seg = this->Clip(other);
    return seg.GetLength();
}

Segment2D Rectangle2D::Clip(Segment2D other) const {

    float top = this->mCorner1.YValue;
    float bottom = this->mCorner2.YValue;
    float left = this->mCorner1.XValue;
    float right = this->mCorner2.XValue;
    Vector2D p1 = other.GetStart();
    Vector2D p2 = other.GetEnd();

    if (p1.XValue < left && p2.XValue < left)
        return Segment2D();                // Both ends left of rectangle - empty segment
    if (p1.XValue >= right && p2.XValue >= right)
        return Segment2D();                // Both ends right of rectangle - empty segment
    if (p1.YValue >= bottom && p2.YValue >= bottom)
        return Segment2D();                // Both ends below rectangle - empty segment
    if (p1.YValue < top && p2.YValue < top)
        return Segment2D();                // Both ends above rectangle - empty segment

    if (this->intersects(Point2D(p1)) && this->intersects(Point2D(p2)))
        return other;                    // Entirely inside rectangle - return entire segment

    // Time to get serious

    float maxT = 1.0;
    float minT = 0.0;
    float tempT = 0;

    // Check for clip at the left edge
    if (p1.XValue < left && p2.XValue >= left) {
        minT = other.GetTFromX(left);
    }

    if (p2.XValue < left && p1.XValue >= left) {
        maxT = other.GetTFromX(left);
    }

    // Check for clip at the right edge
    if (p1.XValue <= right && p2.XValue >= right) {
        tempT = other.GetTFromX(right);
        if (tempT < maxT) maxT = tempT;
    }

    if (p2.XValue <= right && p1.XValue >= right) {
        tempT = other.GetTFromX(right);
        if (tempT > minT) minT = tempT;
    }

    // Check for clip at the top edge
    if (p1.YValue < top && p2.YValue >= top) {
        tempT = other.GetTFromY(top);
        if (tempT > minT) minT = tempT;
    }

    if (p2.YValue < top && p1.YValue >= top) {
        tempT = other.GetTFromY(top);
        if (tempT < maxT) maxT = tempT;
    }

    // Check for clip at bottom edge
    if (p1.YValue <= bottom && p2.YValue >= bottom) {
        tempT = other.GetTFromY(bottom);
        if (tempT < maxT) maxT = tempT;
    }

    if (p2.YValue <= bottom && p1.YValue >= bottom) {
        tempT = other.GetTFromY(bottom);
        if (tempT > minT) minT = tempT;
    }

    // Check for mint and maxt overlap - means you missed the rectangle
    if (minT > maxT) {
        other.PlaceAt(Vector2D(0, 0), Vector2D(0, 0));
    } else {
        other.ClipTo(minT, maxT);
    }

    return other;
}
