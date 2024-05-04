#include "utils/Color.hpp"

#ifndef __ITEXTURE_HPP__
    #define __ITEXTURE_HPP__

namespace Raytracer
{
    namespace Interfaces
    {
        class ITexture {
          public:
            virtual ~ITexture() = default;
            virtual Utils::Color value(
                double u, double v, const Utils::Point3 &point) const = 0;
        };
    } // namespace Interfaces
} // namespace Raytracer

#endif /* __ITEXTURE_HPP__ */
