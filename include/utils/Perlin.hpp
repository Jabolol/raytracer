#include "utils/Color.hpp"
#include "utils/VecN.hpp"

#ifndef __PERLIN_HPP__
    #define __PERLIN_HPP__

namespace Raytracer
{
    namespace Utils
    {
        class Perlin {
          private:
            static constexpr int pointCount = 256;
            Utils::Vec3 *_randVec;
            int *_permX;
            int *_permY;
            int *_permZ;

          public:
            Perlin();
            ~Perlin();
            double noise(const Utils::Point3 &point) const;
            double turbulence(const Utils::Point3 &point, int depth = 7) const;
            static int *perlinGeneratePerm();
            static void permute(int *perm, int n);
            static double perlinInterp(
                const Utils::Vec3 c[2][2][2], double u, double v, double w);
        };
    } // namespace Utils
} // namespace Raytracer

#endif /* __PERLIN_HPP__ */
