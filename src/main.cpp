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


double f(const vec3& d) {
    double cos_theta = d.z();
    return cos_theta * cos_theta * cos_theta;
}

double pdf(const vec3& d) {
    return d.z() / kPi;
}

int main() {
    int32_t N = 1000000;

    double sum = 0.0;
    for (int32_t i = 0; i < N; i++) {
        vec3 d = random_cosine_direction();
        sum += f(d) / pdf(d);
    }

    std::cout << std::fixed << std::setprecision(12);
    std::cout << "PI / 2   = " << kPi / 2.0 << '\n';
    std::cout << "Estimate = " << sum / N   << '\n';

    return 0;
}