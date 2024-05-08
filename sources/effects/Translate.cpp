#include "effects/Translate.hpp"

/**
 * @brief Construct a new Translate object.
 *
 * This function constructs a new Translate object with the given object and
 * offset. The object is translated by the given offset.
 *
 * @param object The object to translate.
 * @param offset The offset to translate the object by.
 *
 * @return A new Translate object.
 */
Raytracer::Effects::Translate::Translate(
    std::shared_ptr<Interfaces::IHittable> object, const Utils::Vec3 &offset)
    : _object(object), _offset(offset)
{
    _bbox = object->boundingBox() + offset;
}

/**
 * @brief Check if the ray hits the translated object.
 *
 * This function checks if the ray hits the translated object. The function
 * returns true if the ray hits the translated object. The function returns
 * false if the ray does not hit the translated object. The function updates
 * the payload with the hit information.
 *
 * @param ray The ray to check for hits.
 * @param interval The interval to check for hits.
 * @param payload The payload to update with the hit information.
 *
 * @return true if the ray hits the translated object, false otherwise.
 */
bool Raytracer::Effects::Translate::hit(const Core::Ray &ray,
    Utils::Interval interval, Core::Payload &payload) const
{
    Core::Ray offsetRay(ray.origin() - _offset, ray.direction(), ray.time());

    if (!_object->hit(offsetRay, interval, payload)) {
        return false;
    }

    Utils::Point3 point = payload.point();
    payload.point(point + _offset);

    return true;
}

/**
 * @brief Get the bounding box of the translated object.
 *
 * This function returns the bounding box of the translated object.
 *
 * @return The bounding box of the translated object.
 */
Raytracer::Utils::AxisAlignedBBox
Raytracer::Effects::Translate::boundingBox() const
{
    return _bbox;
}
