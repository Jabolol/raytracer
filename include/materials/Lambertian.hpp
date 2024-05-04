#include <memory>
#include "interfaces/IMaterial.hpp"
#include "interfaces/ITexture.hpp"

#ifndef __LAMBERTIAN_HPP__
    #define __LAMBERTIAN_HPP__

namespace Raytracer::Materials
{
    class Lambertian : public Interfaces::IMaterial {
      private:
        std::shared_ptr<Interfaces::ITexture> _texture;

      public:
        Lambertian(const Utils::Color &albedo);
        Lambertian(std::shared_ptr<Interfaces::ITexture> texture);
        bool scatter(const Core::Ray &ray, const Core::Payload &payload,
            Utils::Color &attenuation, Core::Ray &scattered) const override;
        Utils::Color emitted(
            double u, double v, const Utils::Point3 &point) const override;
    };
} // namespace Raytracer::Materials

#endif /* __LAMBERTIAN_HPP__ */
