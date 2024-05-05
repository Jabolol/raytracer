#include "interfaces/IMaterial.hpp"

#ifndef __DIELECTRIC_HPP__
    #define __DIELECTRIC_HPP__

namespace Raytracer::Materials
{
    class Dielectric : public Interfaces::IMaterial {
      private:
        double _refractionIndex;
        Utils::Color _albedo = Utils::Color(1.0, 1.0, 1.0);

      public:
        Dielectric(double refractionIndex);
        Dielectric(double refractionIndex, const Utils::Color &albedo);
        bool scatter(const Core::Ray &ray, const Core::Payload &payload,
            Utils::Color &attenuation, Core::Ray &scattered) const override;
        Utils::Color emitted(
            double u, double v, const Utils::Point3 &point) const override;
        static double reflectance(double cosine, double index);
    };
} // namespace Raytracer::Materials

#endif /* __DIELECTRIC_HPP__ */
