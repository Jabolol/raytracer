#include "interfaces/ITexture.hpp"

#ifndef __SOLIDCOLOR_HPP__
    #define __SOLIDCOLOR_HPP__

namespace Raytracer::Textures
{
    class SolidColor : public Interfaces::ITexture {
      private:
        Utils::Color _albedo;

      public:
        SolidColor(const Utils::Color &albedo);
        SolidColor(double red, double green, double blue);
        Utils::Color value(
            double u, double v, const Utils::Point3 &point) const override;
    };
} // namespace Raytracer::Textures

#endif /* __SOLIDCOLOR_HPP__ */
