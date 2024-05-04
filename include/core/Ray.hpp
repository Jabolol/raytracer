#include "Common.hpp"
#include "utils/VecN.hpp"

#ifndef __RAY_HPP__
    #define __RAY_HPP__

namespace Raytracer::Core
{
    class Ray {
      private:
        Utils::Point3 _origin;
        Utils::Vec3 _direction;
        double _time;

      public:
        Ray() = default;
        Ray(const Utils::Point3 &origin, const Utils::Vec3 &direction,
            double time = 0.0);
        Utils::Point3 at(double t) const;
        GET_SET(Utils::Point3, origin)
        GET_SET(Utils::Vec3, direction)
        GET_SET(double, time)
    };
} // namespace Raytracer::Core

#endif /* __RAY_HPP__ */
