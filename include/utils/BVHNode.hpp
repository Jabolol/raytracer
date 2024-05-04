#include "core/Scene.hpp"
#include "interfaces/IHittable.hpp"

#ifndef __BVH_NODE_HPP__
    #define __BVH_NODE_HPP__

namespace Raytracer
{
    namespace Utils
    {
        class BVHNode : public Interfaces::IHittable {
          private:
            std::shared_ptr<Interfaces::IHittable> _left;
            std::shared_ptr<Interfaces::IHittable> _right;
            AxisAlignedBBox _bbox;

          public:
            BVHNode() = default;
            BVHNode(Core::Scene list);
            BVHNode(
                std::vector<std::shared_ptr<Interfaces::IHittable>> &objects,
                size_t start, size_t end);
            bool hit(const Core::Ray &ray, Interval interval,
                Core::Payload &payload) const override;
            AxisAlignedBBox boundingBox() const override;
            static bool boxCompare(
                const std::shared_ptr<Interfaces::IHittable> &a,
                const std::shared_ptr<Interfaces::IHittable> &b, int axis);
            static bool boxXCompare(
                const std::shared_ptr<Interfaces::IHittable> &a,
                const std::shared_ptr<Interfaces::IHittable> &b);
            static bool boxYCompare(
                const std::shared_ptr<Interfaces::IHittable> &a,
                const std::shared_ptr<Interfaces::IHittable> &b);
            static bool boxZCompare(
                const std::shared_ptr<Interfaces::IHittable> &a,
                const std::shared_ptr<Interfaces::IHittable> &b);
            GET_SET(std::shared_ptr<Interfaces::IHittable>, left)
            GET_SET(std::shared_ptr<Interfaces::IHittable>, right)
        };
    } // namespace Utils
} // namespace Raytracer

#endif /* __BVH_NODE_HPP__ */
