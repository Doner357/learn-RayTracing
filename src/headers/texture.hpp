#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <memory>
#include <cmath>

#include "color.hpp"
#include "point3.hpp"

class texture {
    public:
        virtual ~texture() = default;

        virtual color value(double u, double v, const point3& p) const = 0;
};


// Texture only contains constant color on every pixel
class solid_color : public texture {
    public:
        solid_color(const color& albedo) : albedo(albedo) {}

        solid_color(double red, double green, double blue) : solid_color(color(red, green, blue)) {}

        color value(double u, double v, const point3& p) const override {
            return albedo;
        }
    
    private:
        color albedo;
};


// Checker Texture
class checker_texture : public texture {
    public:
        checker_texture(double scale, std::shared_ptr<texture> even, std::shared_ptr<texture> odd) :
            inv_scale(1.0 / scale), even(even), odd(odd) {}

        checker_texture(double scale, const color& c1, const color& c2) :
            inv_scale(1.0 / scale),
            even(std::make_shared<solid_color>(c1)),
            odd(std::make_shared<solid_color>(c2))
            {}

        color value(double u, double v, const point3& p) const override {
            int32_t x_integer = static_cast<int>(std::floor(inv_scale * p.x()));
            int32_t y_integer = static_cast<int>(std::floor(inv_scale * p.y()));
            int32_t z_integer = static_cast<int>(std::floor(inv_scale * p.z()));

            bool is_even = (x_integer + y_integer + z_integer) % 2 == 0;

            return is_even ? even->value(u, v, p) : odd->value(u, v, p);
        }


    private:
        double inv_scale;
        std::shared_ptr<texture> even;
        std::shared_ptr<texture> odd;
};

#endif