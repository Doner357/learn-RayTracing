/*
*
* This file declares and implements interval class to define
* the range of real value.
*
*/

#ifndef INTERVAL_HPP
#define INTERVAL_HPP

#include "constants.hpp"

class interval {
    public:
        double min, max;

        interval() : min(+kInfinity), max(-kInfinity) {} // Default is empty

        interval(double min, double max) : min(min), max(max) {}

        double size() const {
            return max - min;
        }

        bool contains(double x) const {
            return min <= x && x <= max;
        }

        bool surrounds(double x) const {
            return min < x && x < max;
        }

        static const interval empty, universe;
};

const interval interval::empty    = interval(+kInfinity, -kInfinity);
const interval interval::universe = interval(-kInfinity, +kInfinity);

#endif // INTERVAL_HPP