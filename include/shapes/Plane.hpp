#include <memory.h>
#include "interfaces/IHittable.hpp"
#include "utils/VecN.hpp"

#ifndef __PLANE_HPP__
    #define __PLANE_HPP__

namespace Raytracer::Shapes
{
    class Plane : public Interfaces::IHittable {
      private:
        Utils::Point3 _point;
        Utils::Vec3 _normal;
        std::shared_ptr<Interfaces::IMaterial> _material;
        Utils::AxisAlignedBBox _bbox;

      public:
        Plane(const Utils::Point3 &point, const Utils::Vec3 &normal,
            std::shared_ptr<Interfaces::IMaterial> material);
        bool hit(const Core::Ray &ray, Utils::Interval interval,
            Core::Payload &payload) const override;
        Utils::AxisAlignedBBox boundingBox() const override;
    };
} // namespace Raytracer::Shapes
#endif /* __PLANE_HPP__ */
