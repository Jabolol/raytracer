#include <memory>
#include "interfaces/IHittable.hpp"

#ifndef __CONE_HPP__
    #define __CONE_HPP__

namespace Raytracer::Shapes
{
    class Cone : public Interfaces::IHittable {
      private:
        Utils::Point3 _center;
        double _radius;
        double _height;
        std::shared_ptr<Interfaces::IMaterial> _material;
        Utils::AxisAlignedBBox _bbox;

      public:
        Cone() = default;
        Cone(const Utils::Point3 &center, double radius, double height,
            std::shared_ptr<Interfaces::IMaterial> material);
        virtual bool hit(const Core::Ray &ray, Utils::Interval interval,
            Core::Payload &payload) const override;
        virtual Utils::AxisAlignedBBox boundingBox() const override;
    };
} // namespace Raytracer::Shapes

#endif /* __CONE_HPP__ */
