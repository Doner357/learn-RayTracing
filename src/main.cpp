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

struct sample {
    double x;
    double p_x;
};

bool compare_by_x(const sample& a, const sample& b) {
    return a.x < b.x;
}

int main() {
    uint32_t N = 10000;
    double sum = 0.0;

    // Iterate through all of our samples
    std::vector<sample> samples;
    for (uint32_t i = 0; i < N; ++i) {
        // Get the area under the curve
        double x = random_double(0, 2 * kPi);
        double sin_x = std::sin(x);
        double p_x = std::exp(-x / (2 * kPi)) * sin_x * sin_x;
        sum += p_x;
        // store this sample
        sample this_sample =  {x, p_x};
        samples.push_back(this_sample);
    }

    // Sort the samples by x
    std::sort(samples.begin(), samples.end(), compare_by_x);

    // Find out the sample at which we have half of our area
    double half_sum = sum / 2.0;
    double halfway_point = 0.0;
    double accum = 0.0;
    for (uint32_t i =0; i < N; ++i) {
        accum += samples[i].p_x;
        if (accum >= half_sum) {
            halfway_point = samples[i].x;
            break;
        }
    }

    std::cout << std::fixed << std::setprecision(12);
    std::cout << "Average = " << sum / N << '\n';
    std::cout << "Area under curve = " << 2 * kPi * sum / N << '\n';
    std::cout << "Halfway = " << halfway_point << '\n';
}