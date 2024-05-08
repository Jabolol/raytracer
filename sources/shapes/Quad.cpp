#include "shapes/Quad.hpp"
#include <memory>

/**
 * @brief Construct a new Quad object.
 *
 * This function constructs a new Quad object with the given point, u, v, and
 * material. The quad is centered at the given point with the given u, v, and
 * material.
 *
 * @param Q The point of the quad.
 * @param u The u vector of the quad.
 * @param v The v vector of the quad.
 * @param material The material of the quad.
 *
 * @return A new Quad object.
 */
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

/**
 * @brief Check if the ray hits the quad.
 *
 * This function checks if the ray hits the quad. The function returns true if
 * the ray hits the quad. The function returns false if the ray does not hit
 * the quad. The function updates the payload with the hit information.
 *
 * @param ray The ray to check for hits.
 * @param interval The interval to check for hits.
 * @param payload The payload to update with the hit information.
 *
 * @return true if the ray hits the quad, false otherwise.
 */
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

/**
 * @brief Get the bounding box of the quad.
 *
 * This function returns the bounding box of the quad.
 *
 * @return The bounding box of the quad.
 */
Raytracer::Utils::AxisAlignedBBox Raytracer::Shapes::Quad::boundingBox() const
{
    return _bbox;
}

/**
 * @brief Set the bounding box of the quad.
 *
 * This function sets the bounding box of the quad.
 */
void Raytracer::Shapes::Quad::setBBox()
{
    Utils::AxisAlignedBBox diagOne = Utils::AxisAlignedBBox(_Q, _Q + _u + _v);
    Utils::AxisAlignedBBox diagTwo = Utils::AxisAlignedBBox(_Q + _u, _Q + _v);

    _bbox = Utils::AxisAlignedBBox(diagOne, diagTwo);
}

/**
 * @brief Check if the point is inside the quad.
 *
 * This function checks if the point is inside the quad. The function returns
 * true if the point is inside the quad. The function returns false if the
 * point is not inside the quad. The function updates the payload with the u
 * and v values of the point.
 *
 * @param a The alpha value of the point.
 * @param b The beta value of the point.
 * @param payload The payload to update with the u and v values.
 *
 * @return True if the point is inside the quad, false otherwise.
 */
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

/**
 * @brief Construct a box object.
 *
 * This function constructs a box object with the given a, b, and material. The
 * box is centered at the given a and b with the given material.
 *
 * @param a The first point of the box.
 * @param b The second point of the box.
 * @param material The material of the box.
 *
 * @return A new box object.
 */
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
