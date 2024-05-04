#include "interfaces/IHittable.hpp"
#include "utils/AxisAlignedBBox.hpp"
#include "utils/VecN.hpp"

#ifndef __TRANSLATE_HPP__
    #define __TRANSLATE_HPP__

namespace Raytracer::Effects
{
    class Translate : public Interfaces::IHittable {
      private:
        std::shared_ptr<Interfaces::IHittable> _object;
        Utils::Vec3 _offset;
        Utils::AxisAlignedBBox _bbox;

      public:
        Translate(std::shared_ptr<Interfaces::IHittable> object,
            const Utils::Vec3 &offset);
        bool hit(const Core::Ray &ray, Utils::Interval interval,
            Core::Payload &payload) const override;
        Utils::AxisAlignedBBox boundingBox() const override;
    };
} // namespace Raytracer::Effects

#endif /* __TRANSLATE_HPP__ */
