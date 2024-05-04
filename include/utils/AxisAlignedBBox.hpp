#include "core/Ray.hpp"
#include "utils/Interval.hpp"
#include "utils/VecN.hpp"

#ifndef __AXIS_ALIGNED_BBOX_HPP__
    #define __AXIS_ALIGNED_BBOX_HPP__

namespace Raytracer::Utils
{
    class AxisAlignedBBox {
      private:
        Interval _x;
        Interval _y;
        Interval _z;

      public:
        AxisAlignedBBox() = default;
        AxisAlignedBBox(
            const Interval &x, const Interval &y, const Interval &z);
        AxisAlignedBBox(const Point3 &a, const Point3 &b);
        AxisAlignedBBox(const AxisAlignedBBox &a, const AxisAlignedBBox &b);
        const Interval &axisInterval(int n) const;
        bool hit(const Core::Ray &ray, Interval interval) const;
        int longestAxis() const;
        void padToMinimum();
        static const AxisAlignedBBox Empty;
        static const AxisAlignedBBox Universe;
        GET_SET(Interval, x)
        GET_SET(Interval, y)
        GET_SET(Interval, z)
    };

    Utils::AxisAlignedBBox operator+(
        const Utils::AxisAlignedBBox &value, Utils::Vec3 offset);
    Utils::AxisAlignedBBox operator+(
        Utils::Vec3 offset, const Utils::AxisAlignedBBox &value);
} // namespace Raytracer::Utils

#endif /* __AXIS_ALIGNED_BBOX_HPP__ */
