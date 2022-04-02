// Shapes Collection
// Chris Rook
// Last modified 06/11/2018
//    Added AngledRectangle2D and Intersects(AngledRectangle2D) method for other classes

#pragma once

#include "Vector2D.h"

// Abstract 2D shape
class IShape2D {
public:
	virtual ~IShape2D();

	virtual void emplace(const Vector2D& pos, const Vector2D& rot) = 0;
	virtual bool intersects(const IShape2D& other) const = 0;
};

// Predeclaration of all concrete shapes available
class Point2D;
class Segment2D;
class Circle2D;
class Rectangle2D;
class AngledRectangle2D;
