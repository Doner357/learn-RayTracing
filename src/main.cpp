#include <iostream>
#include <cmath>
#include <string>
#include <cstdint>

#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

#include "headers/rtweekend.hpp"
#include "headers/hittable.hpp"
#include "headers/hittable_list.hpp"
#include "headers/sphere.hpp"
#include "headers/canvas.hpp"

// Return the ray attributes as color
color RayColor(const ray& r, const hittable& world);

int main() {

    // Output image name
    std::string image_name = "image";
    // Image type to be written
    Canvas::img_type image_type = Canvas::img_type::ppm  |  // .ppm
                                  Canvas::img_type::png  |  // .png
                                  Canvas::img_type::jpg;    // .jpg


    // -- Image --
    // Image attributes
    double aspect_ratio = 16.0 / 9.0;
    int32_t image_width = 400;

    // Calculate the image height, and ensure that it's at least 1.
    int32_t image_height = static_cast<int32_t>(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;


    // -- World --
    hittable_list world;

    world.add(std::make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5));
    world.add(std::make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0));


    // -- Canmera --
    double focal_length    = 1.0;
    double viewport_height = 2.0;
    double viewport_width  = viewport_height * (static_cast<double>(image_width) / image_height);
    point3 camera_center   = point3(0, 0, 0);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    vec3 viewport_u = vec3(viewport_width, 0, 0);
    vec3 viewport_v = vec3(0, -viewport_height, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    vec3 pixel_delta_u = viewport_u / image_width;
    vec3 pixel_delta_v = viewport_v / image_height;

    // Calculate the location of the upper left pixel.
    point3 viewport_upper_left = camera_center -
                                 vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
    point3 pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);


    // -- Canvas --
    // Canvas to store and write image
    Canvas canvas(image_width, image_height);


    // -- Render --
    for (int32_t j = 0; j < image_height; j++) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int32_t i = 0; i < image_width; i++) {
            point3 pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            vec3 ray_direction  = pixel_center - camera_center;
            ray r(camera_center, ray_direction);

            color pixel_color = RayColor(r, world);

            canvas << pixel_color;
        }
    }
    std::clog << "\rDone.                       \n";

    // Write images
    canvas.writeImage(image_name, image_type);
    
    return 0;
}

color RayColor(const ray& r, const hittable& world) {
    hit_record rec;
    if (world.hit(r, interval(0, kInfinity), rec)) {
        return 0.5 * (rec.normal + color(1.0, 1.0, 1.0));
    }

    vec3 unit_direction = unit_vector(r.direction());
    // Map values ​​[-1.0, 1.0] to [0.0, 1.0]
    double a = 0.5 * (unit_direction.y() + 1.0);
    // Do linear interpolation " blendedValue = (1−a) * startValue + a * endValue "
    // to interpolate the blue and white color according to the y scale.
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}