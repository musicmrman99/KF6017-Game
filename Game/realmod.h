#pragma once

#include <cmath>

// Performs the modulo operation on real numbers.
// Negative modulo flips the modulo direction, eg. realmod(5.0f, -3.0f) => -1.0f,
// because 0 => 0, 1 => -2, 2 => -1, 3 => 0, 4 => -2, 5 => -1.
// Returns INFINITY on division by zero.
inline float realmod(float val, float mod) {
    if (mod == 0) return INFINITY;
    if ((mod > 0 && val >= mod) || val <= mod) { // Same sign, eg. 5 % 3 or -5 % -3
        float div = val / mod;
        return (div - std::floor(div)) * mod;

    } else if ((mod > 0 && val < 0) || val > 0) { // Opposite sign, eg. -1 % 3 or 1 % -3
        float div = val / mod;
        return (1 + div - std::ceil(div)) * mod;

    } else {
        return val;
    }
}

// Performs the modulo operation on real numbers.
// Negative modulo flips the modulo direction, eg. realmod(5.0f, -3.0f) => -1.0f,
// because 0 => 0, 1 => -2, 2 => -1, 3 => 0, 4 => -2, 5 => -1.
// Returns INFINITY on division by zero.
inline double realmod(double val, double mod) {
    if (mod == 0) return INFINITY;
    if ((mod > 0 && val >= mod) || val <= mod) { // Same sign, eg. 5 % 3 or -5 % -3
        double div = val / mod;
        return (div - std::floor(div)) * mod;

    } else if ((mod > 0 && val < 0) || val > 0) { // Opposite sign, eg. -1 % 3 or 1 % -3
        double div = val / mod;
        return (1 + div - std::ceil(div)) * mod;

    } else {
        return val;
    }
}

/*
// TESTS (can be run in https://www.onlinegdb.com/online_c_compiler in C++ mode):

#include <stdio.h>
int main()
{
    printf("%f\n", realmod(3.0f, 5.0f));   //  3.0f
    printf("%f\n", realmod(5.0f, 3.0f));   //  2.0f
    printf("%f\n", realmod(-3.0f, 5.0f));  //  2.0f
    printf("%f\n", realmod(-3.0f, -5.0f)); // -3.0f
    printf("%f\n", realmod(5.0f, -3.0f));  // -1.0f
    printf("%f\n", realmod(-5.0f, -3.0f)); // -2.0f

    return 0;
}
*/
