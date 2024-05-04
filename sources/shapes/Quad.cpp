#include "shapes/Quad.hpp"
#include <memory>

Raytracer::Shapes::Quad::Quad(const Utils::Point3 &Q, const Utils::Vec3 &u,
    const Utils::Vec3 &v, std::shared_ptr<Interfaces::IMaterial> material)
    : _Q(Q), _u(u), _v(v), _material(material)
{
    Utils::Vec3 n = cross(u, v);
    _normal = Utils::unitVector(n);
    _D = dot(_normal, _Q);
    _w = n / dot(n, n);

    setBBox();
}

bool Raytracer::Shapes::Quad::hit(const Core::Ray &ray,
    Utils::Interval interval, Core::Payload &payload) const
{
    double denom = dot(_normal, ray.direction());

    if (std::fabs(denom) < 1e-8) {
        return false;
    }

    double t = (_D - dot(_normal, ray.origin())) / denom;
    if (!interval.contains(t)) {
        return false;
    }

    Utils::Vec3 intersection = ray.at(t);
    Utils::Vec3 planarHit = intersection - _Q;
    double alpha = dot(_w, cross(planarHit, _v));
    double beta = dot(_w, cross(_u, planarHit));

    if (!isInterior(alpha, beta, payload)) {
        return false;
    }

    payload.t(t);
    payload.point(intersection);
    payload.material(_material);
    payload.setFaceNormal(ray, _normal);

    return true;
}

Raytracer::Utils::AxisAlignedBBox Raytracer::Shapes::Quad::boundingBox() const
{
    return _bbox;
}

void Raytracer::Shapes::Quad::setBBox()
{
    Utils::AxisAlignedBBox diagOne = Utils::AxisAlignedBBox(_Q, _Q + _u + _v);
    Utils::AxisAlignedBBox diagTwo = Utils::AxisAlignedBBox(_Q + _u, _Q + _v);

    _bbox = Utils::AxisAlignedBBox(diagOne, diagTwo);
}

bool Raytracer::Shapes::Quad::isInterior(
    double a, double b, Core::Payload &payload) const
{
    Utils::Interval interval = Utils::Interval(0, 1);

    if (!interval.contains(a) || !interval.contains(b)) {
        return false;
    }

    payload.u(a);
    payload.v(b);

    return true;
}

std::shared_ptr<Raytracer::Core::Scene> Raytracer::Shapes::box(
    const Utils::Point3 &a, const Utils::Point3 &b,
    std::shared_ptr<Interfaces::IMaterial> material)
{
    std::shared_ptr<Core::Scene> sides = std::make_shared<Core::Scene>();

    Utils::Point3 min = Utils::Point3(std::fmin(a.x(), b.x()),
        std::fmin(a.y(), b.y()), std::fmin(a.z(), b.z()));
    Utils::Point3 max = Utils::Point3(std::fmax(a.x(), b.x()),
        std::fmax(a.y(), b.y()), std::fmax(a.z(), b.z()));

    Utils::Vec3 dx = Utils::Vec3(max.x() - min.x(), 0, 0);
    Utils::Vec3 dy = Utils::Vec3(0, max.y() - min.y(), 0);
    Utils::Vec3 dz = Utils::Vec3(0, 0, max.z() - min.z());

    sides->add(std::make_shared<Quad>(
        Utils::Point3(min.x(), min.y(), max.z()), dx, dy, material));
    sides->add(std::make_shared<Quad>(
        Utils::Point3(max.x(), min.y(), max.z()), -dz, dy, material));
    sides->add(std::make_shared<Quad>(
        Utils::Point3(max.x(), min.y(), min.z()), -dx, dy, material));
    sides->add(std::make_shared<Quad>(
        Utils::Point3(min.x(), min.y(), min.z()), dz, dy, material));
    sides->add(std::make_shared<Quad>(
        Utils::Point3(min.x(), max.y(), max.z()), dx, -dz, material));
    sides->add(std::make_shared<Quad>(
        Utils::Point3(min.x(), min.y(), min.z()), dx, dz, material));

    return sides;
}
