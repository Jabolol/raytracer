#include "interfaces/ITexture.hpp"

#ifndef __SOLIDCOLOR_HPP__
    #define __SOLIDCOLOR_HPP__

namespace Raytracer
{
    namespace Textures
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
    } // namespace Textures
} // namespace Raytracer

#endif /* __SOLIDCOLOR_HPP__ */
