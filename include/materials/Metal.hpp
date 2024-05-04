#include "interfaces/IMaterial.hpp"

#ifndef __METAL_HPP__
    #define __METAL_HPP__

namespace Raytracer
{
    namespace Materials
    {
        class Metal : public Interfaces::IMaterial {
          private:
            Utils::Color _albedo;
            double _fuzz;

          public:
            Metal(const Utils::Color &albedo, double fuzz);
            bool scatter(const Core::Ray &ray, const Core::Payload &payload,
                Utils::Color &attenuation,
                Core::Ray &scattered) const override;
            Utils::Color emitted(
                double u, double v, const Utils::Point3 &point) const override;
        };
    } // namespace Materials
} // namespace Raytracer

#endif /* __METAL_HPP__ */
