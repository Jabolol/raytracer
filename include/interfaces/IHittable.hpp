#include "core/Payload.hpp"
#include "core/Ray.hpp"
#include "utils/AxisAlignedBBox.hpp"
#include "utils/Interval.hpp"

#ifndef __IHITTABLE_HPP__
    #define __IHITTABLE_HPP__

namespace Raytracer::Interfaces
{
    class IHittable {
      public:
        virtual ~IHittable() = default;
        virtual bool hit(const Core::Ray &ray, Utils::Interval interval,
            Core::Payload &payload) const = 0;
        virtual Utils::AxisAlignedBBox boundingBox() const = 0;
    };
} // namespace Raytracer::Interfaces

#endif /* __IHITTABLE_HPP__ */
