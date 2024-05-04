#include <memory>
#include "interfaces/IHittable.hpp"
#include "utils/VecN.hpp"

#ifndef __SPHERE_HPP__
    #define __SPHERE_HPP__

namespace Raytracer::Shapes
{
    class Sphere : public Interfaces::IHittable {
      private:
        Utils::Point3 _center;
        double _radius;
        std::shared_ptr<Interfaces::IMaterial> _material;
        bool _isMoving = false;
        Utils::Vec3 _centerVec;
        Utils::AxisAlignedBBox _bbox;

      public:
        Sphere(const Utils::Point3 &center, double radius,
            std::shared_ptr<Interfaces::IMaterial> material);
        Sphere(const Utils::Point3 &centerOne, const Utils::Point3 &centerTwo,
            double radius, std::shared_ptr<Interfaces::IMaterial> material);
        bool hit(const Core::Ray &ray, Utils::Interval interval,
            Core::Payload &hit) const override;
        Utils::Point3 sphereCenter(double time) const;
        Utils::AxisAlignedBBox boundingBox() const override;
        static void getSphereUV(
            const Utils::Point3 &point, double &u, double &v);
    };
} // namespace Raytracer::Shapes

#endif /* __SPHERE_HPP__ */
