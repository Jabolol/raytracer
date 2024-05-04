#include "interfaces/ITexture.hpp"
#include "utils/Perlin.hpp"

#ifndef __NOISE_HPP__
    #define __NOISE_HPP__

namespace Raytracer
{
    namespace Textures
    {
        class Noise : public Interfaces::ITexture {
          private:
            double _scale;
            Utils::Perlin _perlin;

          public:
            Noise(double scale);
            Utils::Color value(
                double u, double v, const Utils::Point3 &point) const override;
        };
    } // namespace Textures
} // namespace Raytracer

#endif /* __NOISE_HPP__ */
