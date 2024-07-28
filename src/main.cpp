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

    std::shared_ptr<lambertian> material_ground = std::make_shared<lambertian>(color(0.8, 0.8, 0.0));
    std::shared_ptr<lambertian> material_center = std::make_shared<lambertian>(color(0.1, 0.2, 0.5));
    std::shared_ptr<dielectric> material_left   = std::make_shared<dielectric>(1.0 / 1.33); // Air bubble surrounded by water
    std::shared_ptr<metal>      material_right  = std::make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

    world.add(std::make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(std::make_shared<sphere>(point3( 0.0,    0.0, -1.2),   0.5, material_center));
    world.add(std::make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(std::make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));


    // -- Canmera --
    Camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;

    cam.render(world, "air_bubble");

    
    return 0;
}