#ifndef PDF_HPP
#define PDF_HPP


#include "vec3.hpp"
#include "constants.hpp"
#include "onb.hpp"

class pdf {
    public:
        virtual ~pdf() {}

        virtual double value(const vec3& direction) const = 0;
        virtual vec3 generate() const = 0;
};


class sphere_pdf : public pdf {
    public:
        sphere_pdf() {}

        double value(const vec3& direction) const override {
            return 1 / (4 * kPi);
        }

        vec3 generate() const override {
            return random_unit_vector();
        }
};

class cosine_pdf : public pdf {
    public:
        cosine_pdf(const vec3& w) : uvw(w) {}

        double value(const vec3& direction) const override {
            double cosine_theta = dot(unit_vector(direction), uvw.w());
            return std::fmax(0, cosine_theta / kPi);
        }

        vec3 generate() const override {
            return uvw.transform(random_cosine_direction());
        }


    private:
        onb uvw;
};

#endif // PDF_HPP