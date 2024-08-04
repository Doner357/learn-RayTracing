/*
*
* This file declare and implement sphere class.
*
*/

#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <cmath>
#include <memory>

#include "hittable.hpp"
#include "vec3.hpp"
#include "point3.hpp"


class sphere : public hittable {
    public :
        // Stationary Sphere
        sphere(const point3& center, double radius, std::shared_ptr<material> mat) :
        center1(center), radius(std::max(0.0, radius)), mat(mat), is_moving(false) {
            vec3 rvec = vec3(radius, radius, radius);
            bbox = aabb(center1 - rvec, center1 + rvec);
        }

        // Moving sphere
        sphere(const point3& center1, const point3& center2, double radius,
               std::shared_ptr<material> mat) :
        center1(center1), radius(std::max(0.0, radius)), mat(mat), is_moving(true) {
            vec3 rvec = vec3(radius, radius, radius);
            aabb box1(center1 - rvec, center1 + rvec);
            aabb box2(center2 - rvec, center2 + rvec);
            bbox = aabb(box1, box2);

            center_vec = center2 - center1;
        }

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            // Solve quadratic equation for sphere intersection.
            // Use a simplified solution.
            point3 center = is_moving ? sphere_center(r.time()) : center1;
            vec3 oc = center - r.origin();
            double a = r.direction().length_squared();
            double h = dot(r.direction(), oc);
            double c = oc.length_squared() - radius * radius;

            double discriminant = h * h - a * c;
            if (discriminant < 0) {
                return false;
            }

            double sqrtd = std::sqrt(discriminant);

            // Find the nearest root that lies in the acceptable range.
            double root = (h - sqrtd) / a;
            if (!ray_t.surrounds(root)) {
                root = (h + sqrtd) / a;
                if (!ray_t.surrounds(root)) {
                    return false;
                }
            }

            // Update records
            rec.t = root;
            rec.p = r.at(rec.t);
            vec3 outward_normal = (rec.p - center) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat = mat;

            return true;
        }

        aabb bounding_box() const override { return bbox; }
    
    private:
        point3 center1;
        double radius;
        std::shared_ptr<material> mat;
        bool is_moving;
        vec3 center_vec;
        aabb bbox;

        point3 sphere_center(double time) const {
            // Linearly interpolate from center1 to center2 according to time, where t=0 yields
            // center1, and t=1 yields center2.
            return center1 + time * center_vec;
        }
};

#endif // SPHERE_HPP