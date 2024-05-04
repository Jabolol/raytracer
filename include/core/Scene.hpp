#include <vector>
#include "Common.hpp"
#include "interfaces/IHittable.hpp"

#ifndef __SCENE_HPP__
    #define __SCENE_HPP__

namespace Raytracer::Core
{
    class Scene : public Interfaces::IHittable {
      private:
        Utils::AxisAlignedBBox _bbox;
        std::vector<std::shared_ptr<Interfaces::IHittable>> _objects;

      public:
        Scene() = default;
        Scene(std::shared_ptr<Interfaces::IHittable> object);
        void clear();
        void add(std::shared_ptr<Interfaces::IHittable> object);
        bool hit(const Core::Ray &ray, Utils::Interval interval,
            Core::Payload &payload) const override;
        Utils::AxisAlignedBBox boundingBox() const override;
        GET_SET(std::vector<std::shared_ptr<Interfaces::IHittable>>, objects)
    };
} // namespace Raytracer::Core

#endif /* __SCENE_HPP__ */
