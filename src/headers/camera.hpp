/*
*
* This file declares and implement the camera class, which
* compacts the rendering functions.
*
*/

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <cstdint>

#include "rtweekend.hpp"
#include "canvas.hpp"
#include "hittable.hpp"

class Camera {
    public:
        // -- Image --
        // Image attributes
        double aspect_ratio = 1.0;  // Ratio of image width over height
        int32_t image_width = 100;  // Rendered image width in pixel count


        void render(const hittable& world) {
            // Inittialize
            initialize();

            // -- Canvas --
            // Canvas to store and write image
            Canvas canvas(image_width, image_height);
            // Image type to be written
            Canvas::img_type image_type = Canvas::img_type::ppm  |  // .ppm
                                          Canvas::img_type::png  |  // .png
                                          Canvas::img_type::jpg;    // .jpg

            // -- Render --
            for (int32_t j = 0; j < image_height; j++) {
                std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
                for (int32_t i = 0; i < image_width; i++) {
                    point3 pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
                    vec3 ray_direction  = pixel_center - center;
                    ray r(center, ray_direction);

                    color pixel_color = ray_color(r, world);

                    canvas << pixel_color;
                }
            }
            std::clog << "\rDone.                       \n";

            // Output image name
            std::string image_name = "image";
            // Write images
            canvas.writeImage(image_name, image_type);
        }


    private:
        int32_t image_height;   // Rendered image height
        point3  center;         // Camera center
        point3  pixel00_loc;    // Location of pixel 0, 0
        vec3    pixel_delta_u;  // Offset to pixel to the right
        vec3    pixel_delta_v;  // Offset to pixel below


        void initialize() {
            // Calculate the image height, and ensure that it's at least 1.
            image_height = static_cast<int32_t>(image_width / aspect_ratio);
            image_height = (image_height < 1) ? 1 : image_height;

            center   = point3(0, 0, 0);
            
            double focal_length    = 1.0;
            double viewport_height = 2.0;
            double viewport_width  = viewport_height * (static_cast<double>(image_width) / image_height);

            // Calculate the vectors across the horizontal and down the vertical viewport edges.
            vec3 viewport_u = vec3(viewport_width, 0, 0);
            vec3 viewport_v = vec3(0, -viewport_height, 0);

            // Calculate the horizontal and vertical delta vectors from pixel to pixel.
            pixel_delta_u = viewport_u / image_width;
            pixel_delta_v = viewport_v / image_height;

            // Calculate the location of the upper left pixel.
            point3 viewport_upper_left =
                center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
            pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
        }

        color ray_color(const ray& r, const hittable& world) const {
            hit_record rec;
            // Deal each hittable object
            if (world.hit(r, interval(0, kInfinity), rec)) {
                return 0.5 * (rec.normal + color(1.0, 1.0, 1.0));
            }

            vec3 unit_direction = unit_vector(r.direction());
            double a = 0.5 * (unit_direction.y() + 1.0);
            return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
        }
};

#endif // CAMERA_HPP