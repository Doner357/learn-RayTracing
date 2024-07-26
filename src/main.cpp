#include <iostream>
#include <cmath>
#include <string>
#include <cstdint>

#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

#include "headers/rtweekend.hpp"

#include "headers/camera.hpp"
#include "headers/hittable.hpp"
#include "headers/hittable_list.hpp"
#include "headers/sphere.hpp"

int main() {
    
    // -- World --
    hittable_list world;

    world.add(std::make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5));
    world.add(std::make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0));


    // -- Canmera --
    Camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;

    cam.render(world, "diffuse_infinity");

    
    return 0;
}