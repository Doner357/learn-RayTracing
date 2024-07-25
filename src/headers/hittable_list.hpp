/*
*
* This file declare and implement hittable list class to store a
* list of hittable objects.
*
*/

#include "hittable.hpp"

#include <memory>
#include <vector>

#ifndef HITTABLE_LIST_HPP
#define HITTABLE_LIST_HPP

class hittable_list : public hittable {
    public:
        std::vector<std::shared_ptr<hittable>> objects;

        hittable_list() {}
        hittable_list(std::shared_ptr<hittable> object) { add(object); }

        void clear() { objects.clear(); }

        void add(std::shared_ptr<hittable> object) {
            objects.push_back(object);
        }

        bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const override {
            hit_record temp_rec;
            bool hit_anything = false;
            double closest_so_far = ray_tmax;

            for (const auto& object : objects) {
                if (object->hit(r, ray_tmin, closest_so_far, temp_rec)) {
                    hit_anything = true;
                    closest_so_far = temp_rec.t;
                    rec = temp_rec;
                }
            }

            return hit_anything;
        }
};

#endif // HITTABLE_LIST_HPP