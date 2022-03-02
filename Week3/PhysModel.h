#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

#include "GameTimer.h"

class PhysModel {
private:
    static constexpr float TWO_PI = 2.0f * (float) M_PI; // 2 * pi
    static constexpr float DIST_UNIT = 64.0f;             // Pixels per DIST_UNIT

public:
    static GameTimer gt; // Tracks the time between time marks (for getting FPS)

    virtual ~PhysModel();

    virtual void run() = 0;

    float toDUPS(float mpf); // To distance units/second
    float toRPS(float rpf);  // To revolutions/second
};
