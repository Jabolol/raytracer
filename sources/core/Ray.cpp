#include "core/Ray.hpp"

/**
 * @brief Construct a new Ray object.
 *
 * This function constructs a new Ray object with the given origin, direction,
 * and time.
 *
 * @param origin The origin of the ray.
 * @param direction The direction of the ray.
 * @param time The time of the ray.
 *
 * @return A new Ray object.
 */
Raytracer::Core::Ray::Ray(const Raytracer::Utils::Point3 &origin,
    const Raytracer::Utils::Vec3 &direction, double time)
    : _origin(origin), _direction(direction), _time(time)
{
}

/**
 * @brief Get the origin of the ray.
 *
 * This function returns the origin of the ray.
 *
 * @return The origin of the ray.
 */
Raytracer::Utils::Point3 Raytracer::Core::Ray::at(double t) const
{
    return _origin + t * _direction;
}
