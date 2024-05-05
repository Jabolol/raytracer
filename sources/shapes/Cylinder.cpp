#include "shapes/Cylinder.hpp"

Raytracer::Shapes::Cylinder::Cylinder(const Utils::Point3 &center,
    double radius, double height,
    std::shared_ptr<Interfaces::IMaterial> material)
    : _center(center), _radius(radius), _height(height), _material(material)
{
    _bbox = Utils::AxisAlignedBBox(
        _center - Utils::Vec3(_radius, _height, _radius),
        _center + Utils::Vec3(_radius, _height, _radius));
}

bool Raytracer::Shapes::Cylinder::hit(const Core::Ray &ray,
    Utils::Interval interval, Core::Payload &payload) const
{
    // TODO: Implement cylinder caps intersection
    Utils::Vec3 direction = ray.direction();
    Utils::Point3 origin = ray.origin();

    double a = std::pow(direction.x(), 2) + std::pow(direction.z(), 2);
    double h = 2
        * (direction.x() * (origin.x() - _center.x())
            + direction.z() * (origin.z() - _center.z()));
    double c = std::pow(origin.x() - _center.x(), 2)
        + std::pow(origin.z() - _center.z(), 2) - std::pow(_radius, 2);

    double discriminant = h * h - 4 * a * c;
    if (discriminant < 0) {
        return false;
    }

    double sqrt = std::sqrt(discriminant);

    double root = (-h - sqrt) / (2 * a);
    if (!interval.surrounds(root)) {
        root = (-h + sqrt) / (2 * a);
        if (!interval.surrounds(root)) {
            return false;
        }
    }

    double r = origin.y() + root * direction.y();

    if (r < _center.y() || r > _center.y() + _height) {
        return false;
    }

    payload.t(root);
    payload.point(ray.at(payload.t()));

    Utils::Vec3 normal = (payload.point() - _center);
    normal[1] = 0;
    normal = normal.normalize();

    payload.setFaceNormal(ray, normal);
    payload.material(_material);

    return true;
}

Raytracer::Utils::AxisAlignedBBox
Raytracer::Shapes::Cylinder::boundingBox() const
{
    return _bbox;
}
