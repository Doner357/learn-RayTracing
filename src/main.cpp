#include <iostream>
#include <cmath>
#include <string>
#include <cstdint>
#include <memory>

#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

#include "headers/rtweekend.hpp"

#include "headers/camera.hpp"
#include "headers/hittable.hpp"
#include "headers/hittable_list.hpp"
#include "headers/material.hpp"
#include "headers/sphere.hpp"

int main() {
    
    // -- World --
    hittable_list world;

    double R = std::cos(kPi / 4);

    std::shared_ptr<lambertian> material_left  = std::make_shared<lambertian>(color(0.0, 0.0, 1.0));
    std::shared_ptr<lambertian> material_right = std::make_shared<lambertian>(color(1.0, 0.0, 0.0));

    world.add(std::make_shared<sphere>(point3(-R, 0.0, -1.0), R, material_left));
    world.add(std::make_shared<sphere>(point3( R, 0.0, -1.0), R, material_right));


    // -- Canmera --
    Camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;

    cam.vfov = 90.0;    // FOV is 90 degrees

    cam.render(world, "wide-angle_view");

    
    return 0;
}