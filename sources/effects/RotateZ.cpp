#include "effects/RotateZ.hpp"

/**
 * @brief Construct a new RotateZ object.
 *
 * This function constructs a new RotateZ object with the given object and
 * angle. The object is rotated around the z-axis by the given angle. The
 * bounding box of the object is updated with the rotated bounding box.
 *
 * @param object The object to rotate.
 * @param angle The angle to rotate the object by.
 *
 * @return A new RotateZ object.
 */
Raytracer::Effects::RotateZ::RotateZ(
    std::shared_ptr<Interfaces::IHittable> object, double angle)
    : _object(object)
{
    double infinity = std::numeric_limits<double>::infinity();
    double radians = Utils::degreesToRadians(angle);

    _sinTheta = std::sin(radians);
    _cosTheta = std::cos(radians);
    _bbox = object->boundingBox();

    Utils::Point3 min(infinity, infinity, infinity);
    Utils::Point3 max(-infinity, -infinity, -infinity);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                double x = i * _bbox.x().max() + (1 - i) * _bbox.x().min();
                double y = j * _bbox.y().max() + (1 - j) * _bbox.y().min();
                double z = k * _bbox.z().max() + (1 - k) * _bbox.z().min();

                double newX = _cosTheta * x - _sinTheta * y;
                double newY = _sinTheta * x + _cosTheta * y;

                Utils::Vec3 temp(newX, newY, z);

                for (int l = 0; l < 3; l++) {
                    min[l] = std::fmin(min[l], temp[l]);
                    max[l] = std::fmax(max[l], temp[l]);
                }
            }
        }
    }
    _bbox = Utils::AxisAlignedBBox(min, max);
}

/**
 * @brief Check if the ray hits the rotated object.
 *
 * This function checks if the ray hits the rotated object. The function
 * returns true if the ray hits the rotated object. The function returns false
 * if the ray does not hit the rotated object. The function updates the payload
 * with the hit information.
 *
 * @param ray The ray to check for hits.
 * @param interval The interval to check for hits.
 */
bool Raytracer::Effects::RotateZ::hit(const Core::Ray &ray,
    Utils::Interval interval, Core::Payload &payload) const
{
    Utils::Vec3 origin = ray.origin();
    Utils::Vec3 direction = ray.direction();

    origin[0] = _cosTheta * ray.origin().x() - _sinTheta * ray.origin().y();
    origin[1] = _sinTheta * ray.origin().x() + _cosTheta * ray.origin().y();

    direction[0] =
        _cosTheta * ray.direction().x() - _sinTheta * ray.direction().y();
    direction[1] =
        _sinTheta * ray.direction().x() + _cosTheta * ray.direction().y();

    Core::Ray rotated = Core::Ray(origin, direction, ray.time());

    if (!_object->hit(rotated, interval, payload)) {
        return false;
    }

    Utils::Point3 point = payload.point();
    point[0] =
        _cosTheta * payload.point().x() - _sinTheta * payload.point().y();
    point[1] =
        _sinTheta * payload.point().x() + _cosTheta * payload.point().y();

    Utils::Vec3 normal = payload.normal();
    normal[0] =
        _cosTheta * payload.normal().x() - _sinTheta * payload.normal().y();
    normal[1] =
        _sinTheta * payload.normal().x() + _cosTheta * payload.normal().y();

    payload.point(point);
    payload.normal(normal);

    return true;
}

/**
 * @brief Get the bounding box of the rotated object.
 *
 * This function returns the bounding box of the rotated object.
 *
 * @return The bounding box of the rotated object.
 */
Raytracer::Utils::AxisAlignedBBox
Raytracer::Effects::RotateZ::boundingBox() const
{
    return _bbox;
}
