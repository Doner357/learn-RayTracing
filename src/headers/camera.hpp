/*
*
* This file declares and implement the camera class, which
* compacts the rendering functions.
*
*/

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <cstdint>
#include <string>

#include "rtweekend.hpp"
#include "canvas.hpp"
#include "hittable.hpp"

class Camera {
    public:
        // -- Image --
        // Image attributes
        double   aspect_ratio      = 1.0;  // Ratio of image width over height
        uint32_t image_width       = 100;  // Rendered image width in pixel count
        uint32_t samples_per_pixel = 10;   // Count of random samples for each pixel
        int32_t  max_depth         = 10;   // Maximum number of ray bounces into scene


        void render(const hittable& world, std::string image_name) {
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
            for (uint32_t j = 0; j < image_height; j++) {
                std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
                for (uint32_t i = 0; i < image_width; i++) {
                    color pixel_color(0.0, 0.0, 0.0);
                    for (uint32_t sample = 0; sample < samples_per_pixel; ++sample) {
                        ray r = get_ray(i, j);
                        pixel_color += ray_color(r, max_depth, world);
                    }

                    canvas << pixel_color * pixel_samples_scale;
                }
            }
            std::clog << "\rDone.                       \n";

            // Write images
            canvas.writeImage(image_name, image_type);
        }


    private:
        uint32_t image_height;         // Rendered image height
        double   pixel_samples_scale;  // Color scale factor for a sum of pixel samples
        point3   center;               // Camera center
        point3   pixel00_loc;          // Location of pixel 0, 0
        vec3     pixel_delta_u;        // Offset to pixel to the right
        vec3     pixel_delta_v;        // Offset to pixel below


        void initialize() {
            // Calculate the image height, and ensure that it's at least 1.
            image_height = static_cast<uint32_t>(image_width / aspect_ratio);
            image_height = (image_height < 1) ? 1 : image_height;

            pixel_samples_scale = 1.0 / samples_per_pixel;

            center = point3(0, 0, 0);
            
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

        ray get_ray(uint32_t i, uint32_t j) const {
            // Construct a camera ray originating from the origin and directed at randomly sampled
            // point around the pixel location i, j.
            vec3 offset = sample_square();
            point3 pixel_sample = pixel00_loc +
                                  ((i + offset.x()) * pixel_delta_u) +
                                  ((j + offset.y()) * pixel_delta_v);

            point3 ray_origin = center;
            vec3 ray_direction = pixel_sample - ray_origin;

            return ray(ray_origin, ray_direction);
        }

        vec3 sample_square() const {
            // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
            return vec3(random_double() - 0.5, random_double() - 0.5, 0);
        }

        color ray_color(const ray& r, int32_t depth, const hittable& world) const {
            // If we've exceeded the ray bounce limit, no more light is gathered
            if (depth <= 0) {
                return color(0.0, 0.0, 0.0);
            }

            hit_record rec;
            // Deal each hittable object
            if (world.hit(r, interval(0.001, kInfinity), rec)) {    // The min of interval is set to 0.001 to fix shadow acne.
                vec3 direction = rec.normal + random_unit_vector(); // Lambertian Reflection
                return 0.5 * ray_color(ray(rec.p, direction), depth - 1, world);
            }

            vec3 unit_direction = unit_vector(r.direction());
            double a = 0.5 * (unit_direction.y() + 1.0);
            return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
        }
};

#endif // CAMERA_HPP