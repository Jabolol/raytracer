#include "core/Ray.hpp"

Raytracer::Core::Ray::Ray(const Raytracer::Utils::Point3 &origin,
    const Raytracer::Utils::Vec3 &direction, double time)
    : _origin(origin), _direction(direction), _time(time)
{
}

Raytracer::Utils::Point3 Raytracer::Core::Ray::at(double t) const
{
    return _origin + t * _direction;
}
