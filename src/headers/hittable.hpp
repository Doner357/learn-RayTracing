/*
*
* This file declare and abstract interface for hittable object.
*
*/

#ifndef HITTABLE_HPP
#define HITTABLE_HPP

#include <memory>

#include "ray.hpp"
#include "vec3.hpp"
#include "interval.hpp"
#include "aabb.hpp"

class material;

class hit_record {
    public:
        point3 p;
        vec3   normal;
        std::shared_ptr<material> mat;
        double t;
        double u;
        double v;
        bool front_face;    // Store the hit is from front or back

        void set_face_normal(const ray& r, const vec3& outward_normal) {
            // Sets the hit record normal vector
            // NOTE: the parameter `outward_normal` is assumed to have unit length.

            front_face = dot(r.direction(), outward_normal) < 0;
            normal = front_face ? outward_normal : -outward_normal;
        }
};

class hittable {
    public:
        virtual ~hittable() = default;

        virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;

        // Function to get bounding box
        virtual aabb bounding_box() const = 0;
};

#endif // HITTABLE_HPP