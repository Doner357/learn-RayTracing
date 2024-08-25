#include <iostream>
#include <cmath>
#include <string>
#include <cstdint>
#include <memory>
#include <algorithm>
#include <iomanip>
#include <stdlib.h>
#include <vector>

#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

#include <rtw/rtw_stb_image.hpp>

#include "headers/camera.hpp"
#include "headers/hittable.hpp"
#include "headers/hittable_list.hpp"
#include "headers/material.hpp"
#include "headers/constant_medium.hpp"
#include "headers/sphere.hpp"
#include "headers/quad.hpp"
#include "headers/triangle.hpp"
#include "headers/bvh.hpp"
#include "headers/texture.hpp"

double f(double d) {
    return 8.0 * std::pow(d, 1.0 / 3.0);
}

double pdf(double x) {
    return (3.0 / 8.0) * x * x;
}

int main() {
    int32_t N = 1;
    double sum = 0.0;
    for (int32_t i = 0; i < N; ++i) {
        double x = f(random_double());
        sum += x * x / pdf(x);
    }

    std::cout << std::fixed << std::setprecision(12);
    std::cout << "I = " << sum / N << '\n';
}