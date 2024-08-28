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


double f(double r2) {
    // auto x = cos(2*pi*r1)*2*sqrt(r2*(1-r2));
    // auto y = sin(2*pi*r1)*2*sqrt(r2*(1-r2));
    double z = 1 - r2;
    double cos_theta = z;
    return cos_theta * cos_theta * cos_theta;
}

double pdf() {
    return 1.0 / (2.0 * kPi);
}

int main() {
    int32_t N = 1000000;

    double sum = 0.0;
    for (int32_t i = 0; i < N; i++) {
        double r2 = random_double();
        sum += f(r2) / pdf();
    }

    std::cout << std::fixed << std::setprecision(12);
    std::cout << "PI / 2   = " << kPi / 2.0 << '\n';
    std::cout << "Estimate = " << sum / N   << '\n';

    return 0;
}