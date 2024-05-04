#include <memory>
#include "interfaces/IHittable.hpp"
#include "interfaces/IMaterial.hpp"

#ifndef __CYLINDER_HPP__
    #define __CYLINDER_HPP__

namespace Raytracer::Shapes
{
    class Cylinder : public Interfaces::IHittable {
      private:
        Utils::Point3 _center;
        double _radius;
        double _height;
        std::shared_ptr<Interfaces::IMaterial> _material;
        Utils::AxisAlignedBBox _bbox;

      public:
        Cylinder() = default;
        Cylinder(const Utils::Point3 &center, double radius, double height,
            std::shared_ptr<Interfaces::IMaterial> material);
        virtual bool hit(const Core::Ray &ray, Utils::Interval interval,
            Core::Payload &payload) const override;
        virtual Utils::AxisAlignedBBox boundingBox() const override;
    };
} // namespace Raytracer::Shapes

#endif /* __CYLINDER_HPP__ */
