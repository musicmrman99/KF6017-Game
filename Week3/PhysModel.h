#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

#include <memory>

#include "GameTimer.h"
#include "vector2D.h"

/* Phys Model
-------------------------------------------------- */

class PhysModel {
private:
    static constexpr float TWO_PI = 2.0f * (float) M_PI; // 2 * pi
    static constexpr float DIST_UNIT = 64.0f;            // Pixels per DIST_UNIT

public:
    using Ptr = std::shared_ptr<PhysModel>;
    using UPtr = std::unique_ptr<PhysModel>;
    using WPtr = std::weak_ptr<PhysModel>;

    virtual ~PhysModel();

    virtual void run() = 0;

    // Raw Units -> Game Units

    const Vector2D toDUPS(const Vector2D distanceUnitsPerSecond) const;   // pixels/frame -> distance units/second
    const float toDUPS(const float distanceUnitsPerSecond) const;         // pixels/frame -> distance units/second
    const float toRPS(const float rotationPerSecond) const;               // revolutions/frame -> revolutions/second
    Vector2D toDUPS(Vector2D distanceUnitsPerSecond);                     // pixels/frame -> To distance units/second
    float toDUPS(float distanceUnitsPerSecond);                           // pixels/frame -> To distance units/second
    float toRPS(float rotationPerSecond);                                 // revolutions/frame -> revolutions/second

    // Game Units -> Raw Units

    const Vector2D fromDUPS(const Vector2D distanceUnitsPerSecond) const; // distance units/second -> pixels/frame
    const float fromDUPS(const float distanceUnitsPerSecond) const;       // distance units/second -> pixels/frame
    const float fromRPS(const float rotationPerSecond) const;             // revolutions/second -> revolutions/frame
    Vector2D fromDUPS(Vector2D distanceUnitsPerSecond);                   // distance units/second -> pixels/frame
    float fromDUPS(float distanceUnitsPerSecond);                         // distance units/second -> pixels/frame
    float fromRPS(float rotationPerSecond);                               // revolutions/second -> revolutions/frame
};

/* Null Phys Model
-------------------------------------------------- */

class NullPhysModel : public PhysModel {
public:
    virtual void run() override;
};
