#include "materials/Metal.hpp"
#include "core/Payload.hpp"

/**
 * @brief Construct a new Metal object.
 *
 * This function constructs a new Metal object with the given albedo and fuzz.
 * The Metal material scatters light with the given albedo and fuzz.
 *
 * @param albedo The albedo of the Metal material.
 * @param fuzz The fuzz of the Metal material.
 *
 * @return A new Metal object.
 */
Raytracer::Materials::Metal::Metal(const Utils::Color &albedo, double fuzz)
    : _albedo(albedo), _fuzz(fuzz)
{
}

/**
 * @brief Scatter the ray with the Metal material.
 *
 * This function scatters the ray with the Metal material. The function returns
 * true if the ray is scattered. The function returns false if the ray is not
 * scattered. The function updates the attenuation and scattered ray.
 *
 * @param ray The ray to scatter.
 * @param payload The payload of the ray.
 * @param attenuation The attenuation of the ray.
 * @param scattered The scattered ray.
 *
 * @return true if the ray is scattered, false otherwise.
 */
bool Raytracer::Materials::Metal::scatter(const Core::Ray &ray,
    const Core::Payload &payload, Utils::Color &attenuation,
    Core::Ray &scattered) const
{
    Utils::Vec3 reflected = reflect(ray.direction(), payload.normal());

    reflected = Utils::unitVector(reflected)
        + (_fuzz * Utils::randomUnitVector<double, 3>());
    scattered = Core::Ray(payload.point(), reflected, ray.time());
    attenuation = _albedo;
    return (dot(scattered.direction(), payload.normal()) > 0);
}

/**
 * @brief Emitted light of the Metal material.
 *
 * This function returns the emitted light of the Metal material. The function
 * returns the emitted light of the material at the given point.
 *
 * @param u The u coordinate of the texture.
 * @param v The v coordinate of the texture.
 * @param point The point of intersection.
 *
 * @return The emitted light of the material.
 */
Raytracer::Utils::Color Raytracer::Materials::Metal::emitted(
    double u, double v, const Utils::Point3 &point) const
{
    return Utils::Color(0.0, 0.0, 0.0);
}
