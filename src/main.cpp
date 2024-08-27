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

int main() {
    for (int32_t i = 0; i < 200; ++i) {
        double r1 = random_double();
        double r2 = random_double();
        double x  = std::cos(2 * kPi * r1) * 2 * std::sqrt(r2 * (1 - r2));
        double y  = std::sin(2 * kPi * r1) * 2 * std::sqrt(r2 * (1 - r2));
        double z  = 1 - 2 * r2;
        std::cout << x << " " << y << " " << z << '\n';
    }

    return 0;
}