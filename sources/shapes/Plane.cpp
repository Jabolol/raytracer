#include "shapes/Plane.hpp"

Raytracer::Shapes::Plane::Plane(const Utils::Point3 &point,
    const Utils::Vec3 &normal, std::shared_ptr<Interfaces::IMaterial> material)
    : _point(point), _normal(normal), _material(material)
{
    _bbox = Utils::AxisAlignedBBox(_point, _point);
}

bool Raytracer::Shapes::Plane::hit(const Core::Ray &ray,
    Utils::Interval interval, Core::Payload &payload) const
{
    double denom = dot(_normal, ray.direction());

    if (std::fabs(denom) < 1e-8) {
        return false;
    }

    double t = dot(_normal, _point - ray.origin()) / denom;
    if (!interval.contains(t)) {
        return false;
    }

    Utils::Vec3 intersection = ray.at(t);
    payload.t(t);
    payload.point(intersection);
    payload.material(_material);
    payload.setFaceNormal(ray, _normal);

    return true;
}

Raytracer::Utils::AxisAlignedBBox Raytracer::Shapes::Plane::boundingBox() const
{
    return _bbox;
}
