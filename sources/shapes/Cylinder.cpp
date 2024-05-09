#include "shapes/Cylinder.hpp"

/**
 * @brief Construct a new Cylinder object.
 *
 * This function constructs a new Cylinder object with the given center,
 * radius, height, and material. The cylinder is centered at the given center
 * with the given radius, height, and material.
 *
 * @param center The center of the cylinder.
 * @param radius The radius of the cylinder.
 * @param height The height of the cylinder.
 * @param material The material of the cylinder.
 *
 * @return A new Cylinder object.
 */
Raytracer::Shapes::Cylinder::Cylinder(const Utils::Point3 &center,
    double radius, double height,
    std::shared_ptr<Interfaces::IMaterial> material)
    : _center(center), _radius(radius), _height(height), _material(material)
{
    _bbox = Utils::AxisAlignedBBox(
        _center - Utils::Vec3(_radius, _height, _radius),
        _center + Utils::Vec3(_radius, _height, _radius));
}

/**
 * @brief Check if the ray hits the cylinder.
 *
 * This function checks if the ray hits the cylinder. The function returns true
 * if the ray hits the cylinder. The function returns false if the ray does not
 * hit the cylinder. The function updates the payload with the hit information.
 *
 * @param ray The ray to check for hits.
 * @param interval The interval to check for hits.
 * @param payload The payload to update with the hit information.
 *
 * @return true if the ray hits the cylinder, false otherwise.
 */
bool Raytracer::Shapes::Cylinder::hit(const Core::Ray &ray,
    Utils::Interval interval, Core::Payload &payload) const
{
    Utils::Vec3 direction = ray.direction();
    Utils::Point3 origin = ray.origin();

    double t_cap = (_center.y() - origin.y()) / direction.y();
    if (interval.surrounds(t_cap)) {
        Utils::Point3 hit_point = ray.at(t_cap);
        double dist_squared =
            (hit_point.x() - _center.x()) * (hit_point.x() - _center.x())
            + (hit_point.z() - _center.z()) * (hit_point.z() - _center.z());
        if (dist_squared <= _radius * _radius) {
            payload.t(t_cap);
            payload.point(hit_point);
            payload.setFaceNormal(ray, Utils::Vec3(0, 1, 0));
            payload.material(_material);
            return true;
        }
    }

    t_cap = (_center.y() + _height - origin.y()) / direction.y();
    if (interval.surrounds(t_cap)) {
        Utils::Point3 hit_point = ray.at(t_cap);
        double dist_squared =
            (hit_point.x() - _center.x()) * (hit_point.x() - _center.x())
            + (hit_point.z() - _center.z()) * (hit_point.z() - _center.z());
        if (dist_squared <= _radius * _radius) {
            payload.t(t_cap);
            payload.point(hit_point);
            payload.setFaceNormal(ray, Utils::Vec3(0, -1, 0));
            payload.material(_material);
            return true;
        }
    }

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

/**
 * @brief Get the bounding box of the cylinder.
 *
 * This function returns the bounding box of the cylinder.
 *
 * @return The bounding box of the cylinder.
 */
Raytracer::Utils::AxisAlignedBBox
Raytracer::Shapes::Cylinder::boundingBox() const
{
    return _bbox;
}
