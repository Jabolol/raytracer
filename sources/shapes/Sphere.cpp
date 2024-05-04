#include "shapes/Sphere.hpp"
#include <cmath>

Raytracer::Shapes::Sphere::Sphere(const Utils::Point3 &center, double radius,
    std::shared_ptr<Interfaces::IMaterial> material)
    : _center(center), _radius(std::fmax(0, radius)), _material(material),
      _isMoving(false)
{
    Utils::Vec3 rvec = Utils::Vec3(radius, radius, radius);
    _bbox = Utils::AxisAlignedBBox(_center - rvec, _center + rvec);
}

Raytracer::Shapes::Sphere::Sphere(const Utils::Point3 &one,
    const Utils::Point3 &two, double radius,
    std::shared_ptr<Interfaces::IMaterial> material)
    : _center(one), _radius(std::fmax(0, radius)), _material(material),
      _isMoving(true)
{
    Utils::Vec3 rvec = Utils::Vec3(radius, radius, radius);
    Utils::AxisAlignedBBox box1(one - rvec, one + rvec);
    Utils::AxisAlignedBBox box2(two - rvec, two + rvec);

    _bbox = Utils::AxisAlignedBBox(box1, box2);
    _centerVec = two - one;
}

bool Raytracer::Shapes::Sphere::hit(const Core::Ray &ray,
    Utils::Interval interval, Core::Payload &payload) const
{
    Utils::Point3 center = _isMoving ? sphereCenter(ray.time()) : _center;
    Utils::Vec3 oc = center - ray.origin();
    double a = ray.direction().lengthSquared();
    double h = dot(ray.direction(), oc);
    double c = oc.lengthSquared() - std::pow(_radius, 2);

    double discriminant = h * h - a * c;
    if (discriminant < 0) {
        return false;
    }

    double sqrtd = sqrt(discriminant);

    double root = (h - sqrtd) / a;
    if (!interval.surrounds(root)) {
        root = (h + sqrtd) / a;
        if (!interval.surrounds(root)) {
            return false;
        }
    }

    payload.t(root);
    payload.point(ray.at(payload.t()));

    Utils::Vec3 normal = (payload.point() - center) / _radius;

    payload.setFaceNormal(ray, normal);
    getSphereUV(normal, payload.u(), payload.v());
    payload.material(_material);

    return true;
}

Raytracer::Utils::AxisAlignedBBox
Raytracer::Shapes::Sphere::boundingBox() const
{
    return _bbox;
}

Raytracer::Utils::Point3 Raytracer::Shapes::Sphere::sphereCenter(
    double time) const
{
    return _center + time * _centerVec;
}

void Raytracer::Shapes::Sphere::getSphereUV(
    const Utils::Point3 &point, double &u, double &v)
{
    double theta = std::acos(-point.y());
    double phi = std::atan2(-point.z(), point.x()) + M_PI;

    u = phi / (2 * M_PI);
    v = theta / M_PI;
}
