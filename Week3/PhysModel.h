#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

#include <memory>

#include "GameTimer.h"

/* Phys Model
-------------------------------------------------- */

class PhysModel {
private:
    static constexpr float TWO_PI = 2.0f * (float) M_PI; // 2 * pi
    static constexpr float DIST_UNIT = 64.0f;             // Pixels per DIST_UNIT

public:
    using Ptr = std::shared_ptr<PhysModel>;
    using UPtr = std::unique_ptr<PhysModel>;
    using WPtr = std::weak_ptr<PhysModel>;

    virtual ~PhysModel();

    virtual void run() = 0;

    float toDUPS(float mpf); // To distance units/second
    float toRPS(float rpf);  // To revolutions/second
};

/* Null Phys Model
-------------------------------------------------- */

class NullPhysModel : public PhysModel {
public:
    virtual void run() override;
};
