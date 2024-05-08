#include "materials/Dielectric.hpp"
#include "core/Payload.hpp"
#include "utils/Color.hpp"
#include "utils/VecN.hpp"

/**
 * @brief Construct a new Dielectric object.
 *
 * This function constructs a new Dielectric object with the given refraction
 * index.
 *
 * @param refractionIndex The refraction index of the dielectric.
 *
 * @return A new Dielectric object.
 */
Raytracer::Materials::Dielectric::Dielectric(double refractionIndex)
    : _refractionIndex(refractionIndex)
{
}

/**
 * @brief Construct a new Dielectric object.
 *
 * This function constructs a new Dielectric object with the given refraction
 * index and albedo.
 *
 * @param refractionIndex The refraction index of the dielectric.
 * @param albedo The albedo of the dielectric.
 *
 * @return A new Dielectric object.
 */
Raytracer::Materials::Dielectric::Dielectric(
    double refractionIndex, const Utils::Color &albedo)
    : _refractionIndex(refractionIndex), _albedo(albedo)
{
}

/**
 * @brief Scatter the ray with the dielectric material.
 *
 * This function scatters the ray with the dielectric material. The function
 * returns true if the ray is scattered. The function returns false if the ray
 * is not scattered. The function updates the attenuation and scattered ray.
 *
 * @param ray The ray to scatter.
 * @param payload The payload of the ray.
 * @param attenuation The attenuation of the ray.
 * @param scattered The scattered ray.
 *
 * @return true if the ray is scattered, false otherwise.
 */
bool Raytracer::Materials::Dielectric::scatter(const Core::Ray &ray,
    const Core::Payload &payload, Utils::Color &attenuation,
    Core::Ray &scattered) const
{
    attenuation = _albedo;
    double ri =
        payload.frontFace() ? (1.0 / _refractionIndex) : _refractionIndex;

    Utils::Vec3 unitDirection = Utils::unitVector(ray.direction());
    double cosTheta = std::fmin(dot(-unitDirection, payload.normal()), 1.0);
    double sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);

    bool cantRefract = ri * sinTheta > 1.0;
    Utils::Vec3 direction;

    if (cantRefract || reflectance(cosTheta, ri) > Utils::randomDouble()) {
        direction = reflect(unitDirection, payload.normal());
    } else {
        direction = refract(unitDirection, payload.normal(), ri);
    }

    scattered = Core::Ray(payload.point(), direction, ray.time());

    return true;
}

/**
 * @brief Calculate the reflectance of the dielectric material.
 *
 * This function calculates the reflectance of the dielectric material. The
 * function returns the reflectance of the dielectric material.
 *
 * @param cosine The cosine of the angle.
 * @param index The refraction index.
 *
 * @return The reflectance of the dielectric material.
 */
double Raytracer::Materials::Dielectric::reflectance(
    double cosine, double index)
{
    double r0 = (1 - index) / (1 + index);
    r0 = std::pow(r0, 2);

    return r0 + (1 - r0) * std::pow((1 - cosine), 5);
}

/**
 * @brief Calculate the refracted ray.
 *
 * This function calculates the refracted ray. The function returns the
 * refracted ray.
 *
 * @param uv The unit vector.
 * @param normal The normal vector.
 * @param index The refraction index.
 *
 * @return The refracted ray.
 */
Raytracer::Utils::Color Raytracer::Materials::Dielectric::emitted(
    double u, double v, const Utils::Point3 &point) const
{
    return Utils::Color(0.0, 0.0, 0.0);
}
