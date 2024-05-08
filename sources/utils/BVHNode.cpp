#include "utils/BVHNode.hpp"
#include <algorithm>
#include "core/Scene.hpp"

/**
 * @brief Construct a new BVHNode object.
 *
 * This function constructs a new BVHNode object with the given list of
 * objects. The BVHNode is a bounding volume hierarchy node that represents
 * a node in a BVH tree. The BVH tree is a binary tree that is used to
 * accelerate ray tracing. The BVHNode is constructed from the given list of
 * objects.
 *
 * @param list The list of objects.
 *
 * @return A new BVHNode object.
 */
Raytracer::Utils::BVHNode::BVHNode(Raytracer::Core::Scene list)
    : Raytracer::Utils::BVHNode(list.objects(), 0, list.objects().size())
{
}

/**
 * @brief Construct a new BVHNode object.
 *
 * This function constructs a new BVHNode object with the given list of
 * objects, start index, and end index. The BVHNode is a bounding volume
 * hierarchy node that represents a node in a BVH tree. The BVH tree is a
 * binary tree that is used to accelerate ray tracing. The BVHNode is
 * constructed from the given list of objects, start index, and end index.
 *
 * @param objects The list of objects.
 * @param start The start index.
 * @param end The end index.
 *
 * @return A new BVHNode object.
 */
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

/**
 * @brief Check if the ray hits the BVHNode.
 *
 * This function checks if the ray hits the BVHNode. The function returns true
 * if the ray hits the BVHNode. The function returns false if the ray does not
 * hit the BVHNode. The function updates the payload with the hit information.
 *
 * @param ray The ray to check for hits.
 * @param interval The interval to check for hits.
 * @param payload The payload to update with the hit information.
 *
 * @return true if the ray hits the BVHNode, false otherwise.
 */
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

/**
 * @brief Get the bounding box of the BVHNode.
 *
 * This function returns the bounding box of the BVHNode.
 *
 * @return The bounding box of the BVHNode.
 */
Raytracer::Utils::AxisAlignedBBox
Raytracer::Utils::BVHNode::boundingBox() const
{
    return _bbox;
}

/**
 * @brief Compare two objects based on the given axis.
 *
 * This function compares two objects based on the given axis. The function
 * returns true if the first object is less than the second object based on
 * the given axis. The function returns false otherwise.
 *
 * @param a The first object.
 * @param b The second object.
 * @param axis The axis to compare the objects on.
 *
 * @return true if the first object is less than the second object based on the
 * given axis, false otherwise.
 */
bool Raytracer::Utils::BVHNode::boxCompare(
    const std::shared_ptr<Raytracer::Interfaces::IHittable> &a,
    const std::shared_ptr<Raytracer::Interfaces::IHittable> &b, int axis)
{
    Interval aInterval = a->boundingBox().axisInterval(axis);
    Interval bInterval = b->boundingBox().axisInterval(axis);

    return aInterval.min() < bInterval.min();
}

/**
 * @brief Compare two objects based on the x-axis.
 *
 * This function compares two objects based on the x-axis. The function returns
 * true if the first object is less than the second object based on the x-axis.
 * The function returns false otherwise.
 *
 * @param a The first object.
 * @param b The second object.
 *
 * @return true if the first object is less than the second object based on the
 * x-axis, false otherwise.
 */
bool Raytracer::Utils::BVHNode::boxXCompare(
    const std::shared_ptr<Raytracer::Interfaces::IHittable> &a,
    const std::shared_ptr<Raytracer::Interfaces::IHittable> &b)
{
    return boxCompare(a, b, 0);
}

/**
 * @brief Compare two objects based on the y-axis.
 *
 * This function compares two objects based on the y-axis. The function returns
 * true if the first object is less than the second object based on the y-axis.
 * The function returns false otherwise.
 *
 * @param a The first object.
 * @param b The second object.
 *
 * @return true if the first object is less than the second object based on the
 * y-axis, false otherwise.
 */
bool Raytracer::Utils::BVHNode::boxYCompare(
    const std::shared_ptr<Raytracer::Interfaces::IHittable> &a,
    const std::shared_ptr<Raytracer::Interfaces::IHittable> &b)
{
    return boxCompare(a, b, 1);
}

/**
 * @brief Compare two objects based on the z-axis.
 *
 * This function compares two objects based on the z-axis. The function returns
 * true if the first object is less than the second object based on the z-axis.
 * The function returns false otherwise.
 *
 * @param a The first object.
 * @param b The second object.
 *
 * @return true if the first object is less than the second object based on the
 * z-axis, false otherwise.
 */
bool Raytracer::Utils::BVHNode::boxZCompare(
    const std::shared_ptr<Raytracer::Interfaces::IHittable> &a,
    const std::shared_ptr<Raytracer::Interfaces::IHittable> &b)
{
    return boxCompare(a, b, 2);
}
