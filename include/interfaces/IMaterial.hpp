#include "core/Ray.hpp"
#include "utils/VecN.hpp"

#ifndef __IMATERIAL_HPP__
    #define __IMATERIAL_HPP__

namespace Raytracer::Core
{
    class Payload;
} // namespace Raytracer::Core

namespace Raytracer::Interfaces
{
    class IMaterial {
      public:
        virtual ~IMaterial() = default;
        virtual Utils::Color emitted(
            double u, double v, const Utils::Point3 &point) const = 0;
        virtual bool scatter(const Core::Ray &ray,
            const Core::Payload &payload, Utils::Color &attenuation,
            Core::Ray &scattered) const = 0;
    };
} // namespace Raytracer::Interfaces

#endif /* __IMATERIAL_HPP__ */
