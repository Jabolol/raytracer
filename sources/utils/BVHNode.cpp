#include <algorithm>
#include "utils/BVHNode.hpp"
#include "core/Scene.hpp"

Raytracer::Utils::BVHNode::BVHNode(Raytracer::Core::Scene list)
    : Raytracer::Utils::BVHNode(list.objects(), 0, list.objects().size())
{
}

Raytracer::Utils::BVHNode::BVHNode(
    std::vector<std::shared_ptr<Raytracer::Interfaces::IHittable>> &objects,
    size_t start, size_t end)
{
    _bbox = AxisAlignedBBox::Empty;

    for (size_t i = start; i < end; i++) {
        _bbox = AxisAlignedBBox(_bbox, objects[i]->boundingBox());
    }

    int axis = _bbox.longestAxis();

    auto fn = (axis == 0) ? boxXCompare
        : (axis == 1)     ? boxYCompare
                          : boxZCompare;

    std::size_t objectSpan = end - start;

    if (objectSpan == 1) {
        _left = _right = objects[start];
    } else if (objectSpan == 2) {
        _left = objects[start];
        _right = objects[start + 1];
    } else {
        std::sort(objects.begin() + start, objects.begin() + end, fn);
        std::size_t mid = start + objectSpan / 2;

        _left =
            std::make_shared<Raytracer::Utils::BVHNode>(objects, start, mid);
        _right =
            std::make_shared<Raytracer::Utils::BVHNode>(objects, mid, end);
    }
}

bool Raytracer::Utils::BVHNode::hit(const Raytracer::Core::Ray &ray,
    Raytracer::Utils::Interval interval,
    Raytracer::Core::Payload &payload) const
{
    if (!_bbox.hit(ray, interval)) {
        return false;
    }

    bool hitLeft = _left->hit(ray, interval, payload);
    bool hitRight = _right->hit(ray,
        Interval(interval.min(), hitLeft ? payload.t() : interval.max()),
        payload);

    return hitLeft || hitRight;
}

Raytracer::Utils::AxisAlignedBBox
Raytracer::Utils::BVHNode::boundingBox() const
{
    return _bbox;
}

bool Raytracer::Utils::BVHNode::boxCompare(
    const std::shared_ptr<Raytracer::Interfaces::IHittable> &a,
    const std::shared_ptr<Raytracer::Interfaces::IHittable> &b, int axis)
{
    Interval aInterval = a->boundingBox().axisInterval(axis);
    Interval bInterval = b->boundingBox().axisInterval(axis);

    return aInterval.min() < bInterval.min();
}

bool Raytracer::Utils::BVHNode::boxXCompare(
    const std::shared_ptr<Raytracer::Interfaces::IHittable> &a,
    const std::shared_ptr<Raytracer::Interfaces::IHittable> &b)
{
    return boxCompare(a, b, 0);
}

bool Raytracer::Utils::BVHNode::boxYCompare(
    const std::shared_ptr<Raytracer::Interfaces::IHittable> &a,
    const std::shared_ptr<Raytracer::Interfaces::IHittable> &b)
{
    return boxCompare(a, b, 1);
}

bool Raytracer::Utils::BVHNode::boxZCompare(
    const std::shared_ptr<Raytracer::Interfaces::IHittable> &a,
    const std::shared_ptr<Raytracer::Interfaces::IHittable> &b)
{
    return boxCompare(a, b, 2);
}
