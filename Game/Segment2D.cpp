#include "Segment2D.h"

#include <typeinfo>

#include "Point2D.h"
#include "Circle2D.h"
#include "Rectangle2D.h"
#include "AngledRectangle2D.h"

Segment2D::Segment2D() : mStart(0, 0), mEnd(0, 0) {}

/* Specific Getters / Setters
-------------------- */

Vector2D Segment2D::GetStart() const { return mStart; }
Vector2D Segment2D::GetEnd() const { return mEnd; }

float Segment2D::GetLength() const {
    return (mStart - mEnd).magnitude();
}

float Segment2D::GetTFromX(float x) const {
    if (mEnd.XValue - mStart.XValue == 0.0) {
        if (x == mEnd.XValue)
            return 1.0;
        else
            return 0;
    } else
        return (x - mStart.XValue) / (mEnd.XValue - mStart.XValue);
}

float Segment2D::GetTFromY(float y) const {
    if (mEnd.YValue - mStart.YValue == 0.0) {
        if (y == mEnd.YValue)
            return 1.0;
        else
            return 0;
    } else
        return (y - mStart.YValue) / (mEnd.YValue - mStart.YValue);
}

Vector2D Segment2D::PointFromT(float t) const {
    return (mStart + (mEnd - mStart) * t);
}

// ----------

void Segment2D::PlaceAt(const Vector2D& start, const Vector2D& end) {
    this->mStart = start;
    this->mEnd = end;
}

void Segment2D::SetStartTo(float t) {
    mStart.set(mStart.XValue + t * (mEnd.XValue - mStart.XValue), mStart.YValue + t * (mEnd.YValue - mStart.YValue));
}

void Segment2D::SetEndTo(float t) {
    mEnd.set(mStart.XValue + t * (mEnd.XValue - mStart.XValue), mStart.YValue + t * (mEnd.YValue - mStart.YValue));
}

void Segment2D::ClipTo(float minT, float maxT) {
    Vector2D end;
    end.set(mStart.XValue + maxT * (mEnd.XValue - mStart.XValue), mStart.YValue + maxT * (mEnd.YValue - mStart.YValue));

    SetStartTo(minT);
    mEnd = end;
}

/* Implement IShape2D
-------------------- */

void Segment2D::emplace(const Vector2D& pos, const Vector2D& rot) {
    Vector2D size = this->mEnd - this->mStart;
    PlaceAt(
        pos - size / 2,
        pos + size / 2
    );
}

// ----------

bool Segment2D::intersects(const IShape2D& other) const {
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

// Overloads for specific shapes:

bool Segment2D::intersects(const Point2D& other) const {
    return other.intersects(*this);
}

bool Segment2D::intersects(const Segment2D& other) const {
    // Check that lines are not parallel

    if (!ParallelTo(other)) {
        //Find the t (parametric equation parameter) of the
        // point of intersection on this line

        // Find t. Big equation from
        // http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/
        float t1 = ((other.mEnd.XValue - other.mStart.XValue) * (this->mStart.YValue - other.mStart.YValue)
            - (other.mEnd.YValue - other.mStart.YValue) * (this->mStart.XValue - other.mStart.XValue))
            / ((other.mEnd.YValue - other.mStart.YValue) * (this->mEnd.XValue - this->mStart.XValue)
                - (other.mEnd.XValue - other.mStart.XValue) * (this->mEnd.YValue - this->mStart.YValue));

        Vector2D p1(this->PointFromT(t1));        // Point of intersection

        float t2 = other.GetTFromX(p1.XValue);

        if (t1 >= 0 && t1 < 1.0 && t2 >= 0 && t2 < 1.0)    // Intersection
            return true;
        else
            return false;
    } else        // Parallel lines
        return false;
}

bool Segment2D::intersects(const Circle2D& other) const {
    return (Distance(other) < 0);
}

bool Segment2D::intersects(const Rectangle2D& other) const {
    if (GetLength() == 0)    // Special case
    {
        return(other.intersects(Point2D(mEnd)));
    }
    Segment2D clipped = other.Clip(*this);
    return (clipped.GetLength() > 0);
}

bool Segment2D::intersects(const AngledRectangle2D& other) const {
    return other.intersects(*this);
}

/* Other Utils
-------------------- */

Vector2D Segment2D::Intersection(const Point2D& other) const {
    // Special case - line with length of zero
    if (mEnd == mStart)
        return mEnd;

    // If line length is not zero
    else {
        // Get unit direction vector from Start to End of the line
        Vector2D Direction = (mEnd - mStart).unitVector();

        // Projection of the point onto the line
        float dProjection = (other.GetPosition() - mStart) * Direction;

        float t = dProjection / GetLength();
        Vector2D vec = mStart + Direction * dProjection;
        vec = vec * 2;
        if (t < 0)
            return mStart;
        else if (t > 1.0)
            return mEnd;
        else {

            return mStart + Direction * dProjection;
        }
    }    // End if line length is not zero
}

Vector2D Segment2D::Intersection(const Segment2D& other) const {
    // Check that lines are not parallel
    if (!ParallelTo(other)) {
        // Find the t (parametric equation parameter) of the
        // point of intersection on this line

        // Find t. Big equation from
        // http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/
        float t1 = ((other.mEnd.XValue - other.mStart.XValue) * (this->mStart.YValue - other.mStart.YValue)
            - (other.mEnd.YValue - other.mStart.YValue) * (this->mStart.XValue - other.mStart.XValue))
            / ((other.mEnd.YValue - other.mStart.YValue) * (this->mEnd.XValue - this->mStart.XValue)
                - (other.mEnd.XValue - other.mStart.XValue) * (this->mEnd.YValue - this->mStart.YValue));

        Vector2D p1(this->PointFromT(t1));                 // Point of intersection

        float t2 = other.GetTFromX(p1.XValue);

        if (t1 >= 0 && t1 < 1.0 && t2 >= 0 && t2 < 1.0)    // Intersection
            return p1;                                     // Return the point
    }

    // Intersection not on segments or lines parallel
    double distances[4];                                   // Between four points
    distances[0] = other.Distance(Point2D(mStart));
    distances[1] = other.Distance(Point2D(mEnd));
    distances[2] = this->Distance(Point2D(other.mStart));
    distances[3] = this->Distance(Point2D(other.mEnd));

    // Find smallest
    int smallest = 0;
    for (int i = 1; i < 4; i++) {
        if (distances[i] < distances[smallest])
            smallest = i;
    }

    if (smallest == 0)             // My start closest to other segment
        return mStart;
    else if (smallest == 1)        // My end closest to other segment
        return mEnd;
    else if (smallest == 2)        // Other line's start closest to my segment
        return Intersection(Point2D(other.mStart));
    else                           // other line's end closest to my segment
        return Intersection(Point2D(other.mEnd));
}

Vector2D Segment2D::Intersection(const Circle2D& other) const {

    return Intersection(Point2D(other.mCentre));
}

Vector2D Segment2D::Intersection(const Rectangle2D& other) const {
    Segment2D clipped = other.Clip(*this);
    if (clipped.GetLength() > 0)    // If intersects
        return (clipped.PointFromT(0.5)); // return mid-point of the clipped line

    // What if end of line is closest to an edge, not a corner?
    else                        // Not intersects
    {
        double distances[6];    // From each corner to the line and
                                // each line edge to the rectangle

        // Get distances - this is gonna be slow
        distances[0] = Distance(Point2D(other.GetTopLeft()));
        distances[1] = Distance(Point2D(other.GetTopRight()));
        distances[2] = Distance(Point2D(other.GetBottomLeft()));
        distances[3] = Distance(Point2D(other.GetBottomRight()));
        distances[4] = Point2D(mStart).Distance(other);
        distances[5] = Point2D(mEnd).Distance(other);

        // Find smallest
        int smallest = 0;
        for (int i = 1; i < 6; i++) {
            if (distances[i] < distances[smallest])
                smallest = 1;
        }

        if (smallest == 0)    // Top Left
        {
            return Intersection(Point2D(other.GetTopLeft()));
        } else if (smallest == 1)    // Top right
        {
            return Intersection(Point2D(other.GetTopRight()));
        } else if (smallest == 2)    // Bottom left
        {
            return Intersection(Point2D(other.GetBottomLeft()));
        } else if (smallest == 3)    // Bottom right
        {
            return Intersection(Point2D(other.GetBottomRight()));
        } else if (smallest == 4)    // Start of line closest to an edge
        {
            return Point2D(mStart).Intersection(other);
        } else     //End of line closest to an edge
        {
            return Point2D(mEnd).Intersection(other);
        }
    }
}

// ----------

Vector2D Segment2D::FirstIntersection(const Rectangle2D& other) const {
    Segment2D clipped = other.Clip(*this);
    if (clipped.GetLength() == 0)    // No intersection
        return mEnd;
    else
        return clipped.mStart;
}

// ----------

bool Segment2D::ParallelTo(const Segment2D& other) const {
    return ParallelTo(other.mEnd - other.mStart);
}

bool Segment2D::ParallelTo(const Vector2D& other) const {
    Vector2D direction = mEnd - mStart;
    double dotProduct = direction.unitVector() * other.unitVector();
    if (dotProduct < 1.00001 && dotProduct>0.99999)
        return true;
    if (dotProduct > -1.00001 && dotProduct < -0.99999)
        return true;
    else return false;
}

// ----------

float Segment2D::Distance(const Point2D& other) const {
    return (other.GetPosition() - Intersection(other)).magnitude();
}

float Segment2D::Distance(const Segment2D& other) const {
    // Check that lines are not parallel

    if (!ParallelTo(other)) {
        //Find the t (parametric equation parameter) of the
        // point of intersection on this line

        // Find t. Big equation from
        // http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/
        float t1 = ((other.mEnd.XValue - other.mStart.XValue) * (this->mStart.YValue - other.mStart.YValue)
            - (other.mEnd.YValue - other.mStart.YValue) * (this->mStart.XValue - other.mStart.XValue))
            / ((other.mEnd.YValue - other.mStart.YValue) * (this->mEnd.XValue - this->mStart.XValue)
                - (other.mEnd.XValue - other.mStart.XValue) * (this->mEnd.YValue - this->mStart.YValue));

        Vector2D p1(this->PointFromT(t1));        // Point of intersection

        float t2 = other.GetTFromX(p1.XValue);

        if (t1 >= 0 && t1 < 1.0 && t2 >= 0 && t2 < 1.0)    // Intersection
            return 0;                            // Return zero separation
    }

    // Intersection not on segments or lines parallel
    float distances[4];                // Between four points
    distances[0] = other.Distance(Point2D(mStart));
    distances[1] = other.Distance(Point2D(mEnd));
    distances[2] = this->Distance(Point2D(other.mStart));
    distances[3] = this->Distance(Point2D(other.mEnd));

    // Find smallest
    int smallest = 0;
    for (int i = 1; i < 4; i++) {
        if (distances[i] < distances[smallest])
            smallest = i;
    }

    return distances[smallest];
}

float Segment2D::Distance(const Circle2D& other) const {
    // Get distance to the centre and subtract the radius
    return (Distance(Point2D(other.GetCentre())) - other.GetRadius());;
}

float Segment2D::Distance(const Rectangle2D& other) const {
    // Slow
    if (intersects(other))
        return 0.0;
    return Distance(Point2D(Intersection(other)));
}
