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
    Utils::Vec3 oc = ray.origin() - _center;
    double a = ray.direction().lengthSquared()
        - ray.direction().y() * ray.direction().y();
    double b = 2 * (oc.x() - _center.x()) * ray.direction().x()
        + 2 * (oc.z() - _center.z()) * ray.direction().z()
        - 2 * ray.direction().y() * (oc.y() - _center.y());
    double c = (oc.x() - _center.x()) * (oc.x() - _center.x())
        + (oc.z() - _center.z()) * (oc.z() - _center.z()) - _radius * _radius;
    double discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        return false;
    }

    double root = (-b - sqrt(discriminant)) / (2 * a);
    if (!interval.contains(root)) {
        root = (-b + sqrt(discriminant)) / (2 * a);
        if (!interval.contains(root)) {
            return false;
        }
    }

    double y = ray.origin().y() + root * ray.direction().y();
    if (y < _center.y() || y > _center.y() + _height) {
        return false;
    }

    payload.t(root);
    payload.point(ray.at(payload.t()));

    Utils::Vec3 normal = (payload.point() - _center);
    normal[1] = 0;
    normal = Utils::unitVector(normal);

    payload.setFaceNormal(ray, normal);
    payload.material(_material);

    return true;
}

Raytracer::Utils::AxisAlignedBBox
Raytracer::Shapes::Cylinder::boundingBox() const
{
    return _bbox;
}
