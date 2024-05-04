#include "interfaces/IHittable.hpp"
#include "interfaces/ITexture.hpp"

#ifndef __SMOKE_HPP__
    #define __SMOKE_HPP__

namespace Raytracer
{
    namespace Effects
    {
        class Smoke : public Interfaces::IHittable {
          private:
            std::shared_ptr<Interfaces::IHittable> _boundary;
            std::shared_ptr<Interfaces::IMaterial> _phaseFunction;
            double _density;

          public:
            Smoke(std::shared_ptr<Interfaces::IHittable> boundary,
                double density, std::shared_ptr<Interfaces::ITexture> texture);
            Smoke(std::shared_ptr<Interfaces::IHittable> boundary,
                double density, const Utils::Color &albedo);
            bool hit(const Core::Ray &ray, Utils::Interval interval,
                Core::Payload &payload) const override;
            Utils::AxisAlignedBBox boundingBox() const override;
        };
    } // namespace Effects
} // namespace Raytracer

#endif /* __SMOKE_HPP__ */
