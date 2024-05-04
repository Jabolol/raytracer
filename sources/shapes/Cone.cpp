#include "shapes/Cone.hpp"

Raytracer::Shapes::Cone::Cone(const Utils::Point3 &center, double radius,
    double height, std::shared_ptr<Interfaces::IMaterial> material)
    : _center(center), _radius(radius), _height(height), _material(material)
{
    Utils::Point3 min = _center - Utils::Vec3(_radius, _height, _radius);
    Utils::Point3 max = _center + Utils::Vec3(_radius, _height, _radius);
    _bbox = Utils::AxisAlignedBBox(min, max);
}

bool Raytracer::Shapes::Cone::hit(const Core::Ray &ray,
    Utils::Interval interval, Core::Payload &payload) const
{
    Utils::Vec3 oc = ray.origin() - _center;
    double a = ray.direction().x() * ray.direction().x()
        + ray.direction().z() * ray.direction().z()
        - ray.direction().y() * ray.direction().y() * _radius * _radius
            / (_height * _height);
    double b = 2
        * (oc.x() * ray.direction().x() + oc.z() * ray.direction().z()
            - oc.y() * ray.direction().y() * _radius * _radius
                / (_height * _height));
    double c = oc.x() * oc.x() + oc.z() * oc.z()
        - oc.y() * oc.y() * _radius * _radius / (_height * _height);
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
