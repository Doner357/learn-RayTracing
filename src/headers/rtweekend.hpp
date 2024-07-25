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
#include "constants.hpp"

// Utility Functions
inline double DegreesToRadians(double degrees) {
    return degrees * kPi / 180.0;
}

// Common Headers
#include "vec3.hpp"
#include "color.hpp"
#include "point3.hpp"
#include "interval.hpp"
#include "ray.hpp"

#endif // RTWEEKEND_HPP