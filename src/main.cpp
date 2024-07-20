#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <fstream>

#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

int main() {

    // -- Image --

    // Store image color data
    std::vector<uint8_t> image_data;

    int32_t image_width  = 256;
    int32_t image_height = 256;
    int32_t image_comp   = 3;    // R, G, B

    // -- Render --

    for (int32_t j = 0; j < image_height; j++) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int32_t i = 0; i < image_width; i++) {
            double r = static_cast<double>(i) / (image_width - 1);
            double g = static_cast<double>(j) / (image_width - 1);
            double b = 0.0;

            int32_t ir = static_cast<int32_t>(255.999 * r);
            int32_t ig = static_cast<int32_t>(255.999 * g);
            int32_t ib = static_cast<int32_t>(255.999 * b);

            image_data.push_back(ir);
            image_data.push_back(ig);
            image_data.push_back(ib);
        }
    }
    std::clog << "\rDone.                       \n";


    // Output .ppm file
    std::ofstream ppm_image;
    ppm_image.open("ppm_image.ppm");
    // .ppm Header
    ppm_image << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    // .ppm color data
    for (uint32_t i = 0; i < image_data.size(); i += 3) {
        ppm_image << static_cast<int32_t>(image_data[i])     << ' ' <<  // R
                     static_cast<int32_t>(image_data[i + 1]) << ' ' <<  // G
                     static_cast<int32_t>(image_data[i + 2]) << '\n';   // B
    }
    // End output .ppm file
    ppm_image.close();


    // Test stbi_image_write
    // Output png file
    stbi_write_png("png_image.png", image_width, image_height, image_comp, image_data.data(), image_width * image_comp);


    // Test stbi_image
    int load_image_width  = 0,
        load_image_height = 0,
        load_image_comp   = 0;
    // Load image output previously
    unsigned char* load_image_data = stbi_load("png_image.png", &load_image_width, &load_image_height, &load_image_comp, 0);
    if (load_image_data) {
        std::clog << "Load successfully!" << '\n' <<
                     "width:     " << load_image_width  << '\n' <<
                     "height:    " << load_image_height << '\n' <<
                     "component: " << load_image_comp   << std::endl;
    }
    else {
        std::clog << "Failed to load image!" << std::endl;
    }
    stbi_image_free(load_image_data);

    return 0;
}
