#include <memory>
#include "interfaces/IMaterial.hpp"
#include "interfaces/ITexture.hpp"

#ifndef __ISOTROPIC_HPP__
    #define __ISOTROPIC_HPP__

namespace Raytracer
{
    namespace Materials
    {
        class Isotropic : public Interfaces::IMaterial {
          private:
            std::shared_ptr<Interfaces::ITexture> _texture;

          public:
            Isotropic(std::shared_ptr<Interfaces::ITexture> texture);
            Isotropic(const Utils::Color &color);
            bool scatter(const Core::Ray &ray, const Core::Payload &payload,
                Utils::Color &attenuation,
                Core::Ray &scattered) const override;
            Utils::Color emitted(
                double u, double v, const Utils::Point3 &point) const override;
        };
    } // namespace Materials
} // namespace Raytracer

#endif /* __ISOTROPIC_HPP__ */
