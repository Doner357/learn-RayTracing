#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <fstream>

#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

#include "headers/vec3.hpp"
#include "headers/color.hpp"
#include "headers/point3.hpp"
#include "headers/canvas.hpp"

int main() {

    // -- Image --
    // Image attributes
    std::string image_name = "image";
    int32_t image_width  = 256;
    int32_t image_height = 256;
    // Canvas to store and write image
    Canvas canvas(image_width, image_height);
    // Image type to be written
    Canvas::img_type image_type = Canvas::img_type::jpg | Canvas::img_type::ppm | Canvas::img_type::png;

    // -- Render --
    for (int32_t j = 0; j < image_height; j++) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int32_t i = 0; i < image_width; i++) {
            canvas << color(double(i) / (image_width - 1), double(j) / (image_height - 1), 0.0);
        }
    }
    std::clog << "\rDone.                       \n";

    // Write images
    canvas.writeImage(image_name, image_type);
    
    return 0;
}