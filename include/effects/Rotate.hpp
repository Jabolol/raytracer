#include "interfaces/IHittable.hpp"

#ifndef __ROTATE_HPP__
    #define __ROTATE_HPP__

namespace Raytracer::Effects
{
    class Rotate : public Interfaces::IHittable {
      private:
        std::shared_ptr<Interfaces::IHittable> _object;
        double _sinTheta;
        double _cosTheta;
        Utils::AxisAlignedBBox _bbox;

      public:
        Rotate(std::shared_ptr<Interfaces::IHittable> object, double angle);
        bool hit(const Core::Ray &ray, Utils::Interval interval,
            Core::Payload &payload) const override;
        Utils::AxisAlignedBBox boundingBox() const override;
    };

} // namespace Raytracer::Effects

#endif /* __ROTATE_HPP__ */
