#ifndef PERLIN_HPP
#define PERLIN_HPP

#include <cstdint>

#include "rtweekend.hpp"

class perlin {
    public:
        perlin() {
            randfloat = new double[point_count];
            for (int32_t i = 0; i < point_count; ++i) {
                randfloat[i] = random_double();
            }

            perm_x = perlin_generate_perm();
            perm_y = perlin_generate_perm();
            perm_z = perlin_generate_perm();
        }

        ~perlin() {
            delete [] randfloat;
            delete [] perm_x;
            delete [] perm_y;
            delete [] perm_z;
        }

        double noise(const point3& p) const {
            int32_t i = int32_t(4 * p.x()) & 255;
            int32_t j = int32_t(4 * p.y()) & 255;
            int32_t k = int32_t(4 * p.z()) & 255;

            return randfloat[perm_x[i] ^ perm_y[j] ^ perm_z[k]];
        }

    
    private:
        static const int point_count = 256;
        double* randfloat;
        int32_t* perm_x;
        int32_t* perm_y;
        int32_t* perm_z;

        static int32_t* perlin_generate_perm() {
            int32_t* p = new int32_t[point_count];

            for (int32_t i = 0; i < point_count; i++) {
                p[i] = i;
            }

            permute(p, point_count);

            return p;   
        }

        static void permute(int32_t* p, int32_t n) {
            for (int32_t i = n - 1; i > 0; i--) {
                int target = random_int(0, i);
                int tmp = p[i];
                p[target] = tmp;
            }
        }
};

#endif // PERLIN_HPP