#include "shapes/Cone.hpp"

/**
 * @brief Construct a new Cone object.
 *
 * This function constructs a new Cone object with the given center, radius,
 * height, and material. The cone is centered at the given center with the
 * given radius, height, and material.
 *
 * @param center The center of the cone.
 * @param radius The radius of the cone.
 * @param height The height of the cone.
 * @param material The material of the cone.
 *
 * @return A new Cone object.
 */
Raytracer::Shapes::Cone::Cone(const Utils::Point3 &center, double radius,
    double height, std::shared_ptr<Interfaces::IMaterial> material)
    : _center(center), _radius(radius), _height(height), _material(material)
{
    Utils::Point3 min = _center - Utils::Vec3(_radius, _height, _radius);
    Utils::Point3 max = _center + Utils::Vec3(_radius, _height, _radius);
    _bbox = Utils::AxisAlignedBBox(min, max);
}

/**
 * @brief Check if the ray hits the cone.
 *
 * This function checks if the ray hits the cone. The function returns true if
 * the ray hits the cone. The function returns false if the ray does not hit
 * the cone. The function updates the payload with the hit information.
 *
 * @param ray The ray to check for hits.
 * @param interval The interval to check for hits.
 * @param payload The payload to update with the hit information.
 *
 * @return true if the ray hits the cone, false otherwise.
 */
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

    double top_cap = _center.y() + _height;
    double t = ((top_cap - ray.origin().y()) / ray.direction().y());
    if (interval.contains(t)) {
        double x = ray.origin().x() + t * ray.direction().x();
        double z = ray.origin().z() + t * ray.direction().z();
        double distance_squared = (x - _center.x()) * (x - _center.x())
            + (z - _center.z()) * (z - _center.z());
        if (distance_squared <= _radius * _radius) {
            payload.t(t);
            payload.point(ray.at(payload.t()));
            payload.setFaceNormal(ray, Utils::Vec3(0, 1, 0));
            payload.material(_material);
            return true;
        }
    }

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

/**
 * @brief Get the bounding box of the cone.
 *
 * This function returns the bounding box of the cone.
 *
 * @return The bounding box of the cone.
 */
Raytracer::Utils::AxisAlignedBBox Raytracer::Shapes::Cone::boundingBox() const
{
    return _bbox;
}
