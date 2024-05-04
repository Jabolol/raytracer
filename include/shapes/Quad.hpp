#include <memory>
#include "core/Scene.hpp"
#include "interfaces/IHittable.hpp"

#ifndef __QUAD_HPP__
    #define __QUAD_HPP__

namespace Raytracer::Shapes
{
    class Quad : public Interfaces::IHittable {
      private:
        Utils::Point3 _Q;
        Utils::Vec3 _u;
        Utils::Vec3 _v;
        Utils::Vec3 _w;
        std::shared_ptr<Interfaces::IMaterial> _material;
        Utils::AxisAlignedBBox _bbox;
        Utils::Vec3 _normal;
        double _D;

      public:
        Quad(const Utils::Point3 &Q, const Utils::Vec3 &u,
            const Utils::Vec3 &v,
            std::shared_ptr<Interfaces::IMaterial> material);
        bool hit(const Core::Ray &ray, Utils::Interval interval,
            Core::Payload &payload) const override;
        Utils::AxisAlignedBBox boundingBox() const override;
        virtual void setBBox();
        virtual bool isInterior(
            double a, double b, Core::Payload &payload) const;
    };

    std::shared_ptr<Core::Scene> box(const Utils::Point3 &a,
        const Utils::Point3 &b,
        std::shared_ptr<Interfaces::IMaterial> material);
} // namespace Raytracer::Shapes

#endif /* __QUAD_HPP__ */
