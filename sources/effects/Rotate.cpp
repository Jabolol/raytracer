#include "effects/Rotate.hpp"

Raytracer::Effects::Rotate::Rotate(
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

                double newX = _cosTheta * x + _sinTheta * z;
                double newZ = -_sinTheta * x + _cosTheta * z;

                Utils::Vec3 temp(newX, y, newZ);

                for (int l = 0; l < 3; l++) {
                    min[l] = std::fmin(min[l], temp[l]);
                    max[l] = std::fmax(max[l], temp[l]);
                }
            }
        }
    }
    _bbox = Utils::AxisAlignedBBox(min, max);
}

bool Raytracer::Effects::Rotate::hit(const Core::Ray &ray,
    Utils::Interval interval, Core::Payload &payload) const
{
    Utils::Vec3 origin = ray.origin();
    Utils::Vec3 direction = ray.direction();

    origin[0] = _cosTheta * ray.origin().x() - _sinTheta * ray.origin().z();
    origin[2] = _sinTheta * ray.origin().x() + _cosTheta * ray.origin().z();

    direction[0] =
        _cosTheta * ray.direction().x() - _sinTheta * ray.direction().z();
    direction[2] =
        _sinTheta * ray.direction().x() + _cosTheta * ray.direction().z();

    Core::Ray rotated = Core::Ray(origin, direction, ray.time());

    if (!_object->hit(rotated, interval, payload)) {
        return false;
    }

    Utils::Point3 point = payload.point();
    point[0] =
        _cosTheta * payload.point().x() + _sinTheta * payload.point().z();
    point[2] =
        -_sinTheta * payload.point().x() + _cosTheta * payload.point().z();

    Utils::Vec3 normal = payload.normal();
    normal[0] =
        _cosTheta * payload.normal().x() + _sinTheta * payload.normal().z();
    normal[2] =
        -_sinTheta * payload.normal().x() + _cosTheta * payload.normal().z();

    payload.point(point);
    payload.normal(normal);

    return true;
}

Raytracer::Utils::AxisAlignedBBox
Raytracer::Effects::Rotate::boundingBox() const
{
    return _bbox;
}
