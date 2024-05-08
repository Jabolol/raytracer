#include "shapes/Sphere.hpp"
#include <cmath>

/**
 * @brief Construct a new Sphere object.
 *
 * This function constructs a new Sphere object with the given center, radius,
 * and material. The sphere is centered at the given center with the given
 * radius and material.
 *
 * @param center The center of the sphere.
 * @param radius The radius of the sphere.
 * @param material The material of the sphere.
 *
 * @return A new Sphere object.
 */
Raytracer::Shapes::Sphere::Sphere(const Utils::Point3 &center, double radius,
    std::shared_ptr<Interfaces::IMaterial> material)
    : _center(center), _radius(std::fmax(0, radius)), _material(material),
      _isMoving(false)
{
    Utils::Vec3 rvec = Utils::Vec3(radius, radius, radius);
    _bbox = Utils::AxisAlignedBBox(_center - rvec, _center + rvec);
}

/**
 * @brief Construct a new Sphere object.
 *
 * This function constructs a new Sphere object with the given centers, radius,
 * and material. The sphere is centered at the given centers with the given
 * radius and material.
 *
 * @param one The first center of the sphere.
 * @param two The second center of the sphere.
 * @param radius The radius of the sphere.
 * @param material The material of the sphere.
 *
 * @return A new Sphere object.
 */
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

/**
 * @brief Check if the ray hits the sphere.
 *
 * This function checks if the ray hits the sphere. The function returns true
 * if the ray hits the sphere. The function returns false if the ray does not
 * hit the sphere. The function updates the payload with the hit information.
 *
 * @param ray The ray to check for hits.
 * @param interval The interval to check for hits.
 * @param payload The payload to update with the hit information.
 *
 * @return true if the ray hits the sphere, false otherwise.
 */
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

/**
 * @brief Get the bounding box of the sphere.
 *
 * This function returns the bounding box of the sphere.
 *
 * @return The bounding box of the sphere.
 */
Raytracer::Utils::AxisAlignedBBox
Raytracer::Shapes::Sphere::boundingBox() const
{
    return _bbox;
}

/**
 * @brief Get the center of the sphere at the given time.
 *
 * This function returns the center of the sphere at the given time.
 *
 * @param time The time to get the center of the sphere.
 *
 * @return The center of the sphere at the given time.
 */
Raytracer::Utils::Point3 Raytracer::Shapes::Sphere::sphereCenter(
    double time) const
{
    return _center + time * _centerVec;
}

/**
 * @brief Get the UV coordinates of the sphere.
 *
 * This function returns the UV coordinates of the sphere.
 *
 * @param point The point to get the UV coordinates.
 * @param u The U coordinate of the UV coordinates.
 * @param v The V coordinate of the UV coordinates.
 */
void Raytracer::Shapes::Sphere::getSphereUV(
    const Utils::Point3 &point, double &u, double &v)
{
    double theta = std::acos(-point.y());
    double phi = std::atan2(-point.z(), point.x()) + M_PI;

    u = phi / (2 * M_PI);
    v = theta / M_PI;
}
