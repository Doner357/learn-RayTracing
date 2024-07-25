/*
*
* This file contains some math constants, utility functions,
* and general main headers
*
*/

#ifndef RTWEEKEND_HPP
#define RTWEEKEND_HPP

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>

// Constants
const double kInfinity = std::numeric_limits<double>::infinity();
const double kPi       = 3.1415926535897932385;

// Utility Functions
inline double DegreesToRadians(double degrees) {
    return degrees * kPi / 180.0;
}

// Common Headers
#include "vec3.hpp"
#include "color.hpp"
#include "point3.hpp"
#include "ray.hpp"

#endif // RTWEEKEND_HPP