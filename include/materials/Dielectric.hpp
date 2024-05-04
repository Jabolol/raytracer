#include "interfaces/IMaterial.hpp"

#ifndef __DIELECTRIC_HPP__
    #define __DIELECTRIC_HPP__

namespace Raytracer
{
    namespace Materials
    {
        class Dielectric : public Interfaces::IMaterial {
          private:
            double _refractionIndex;

          public:
            Dielectric(double refraction_index);
            bool scatter(const Core::Ray &ray, const Core::Payload &payload,
                Utils::Color &attenuation,
                Core::Ray &scattered) const override;
            Utils::Color emitted(
                double u, double v, const Utils::Point3 &point) const override;
            static double reflectance(double cosine, double index);
        };
    } // namespace Materials
} // namespace Raytracer

#endif /* __DIELECTRIC_HPP__ */
