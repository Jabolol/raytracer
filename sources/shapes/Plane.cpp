#include "shapes/Plane.hpp"

/**
 * @brief Construct a new Plane object.
 *
 * This function constructs a new Plane object with the given point, normal,
 * and material. The plane is centered at the given point with the given normal
 * and material.
 *
 * @param point The point of the plane.
 * @param normal The normal of the plane.
 * @param material The material of the plane.
 *
 * @return A new Plane object.
 */
Raytracer::Shapes::Plane::Plane(const Utils::Point3 &point,
    const Utils::Vec3 &normal, std::shared_ptr<Interfaces::IMaterial> material)
    : _point(point), _normal(normal), _material(material)
{
    _bbox = Utils::AxisAlignedBBox(_point, _point);
}

/**
 * @brief Check if the ray hits the plane.
 *
 * This function checks if the ray hits the plane. The function returns true if
 * the ray hits the plane. The function returns false if the ray does not hit
 * the plane. The function updates the payload with the hit information.
 *
 * @param ray The ray to check for hits.
 * @param interval The interval to check for hits.
 * @param payload The payload to update with the hit information.
 *
 * @return true if the ray hits the plane, false otherwise.
 */
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

/**
 * @brief Get the bounding box of the plane.
 *
 * This function returns the bounding box of the plane.
 *
 * @return The bounding box of the plane.
 */
Raytracer::Utils::AxisAlignedBBox Raytracer::Shapes::Plane::boundingBox() const
{
    return _bbox;
}
