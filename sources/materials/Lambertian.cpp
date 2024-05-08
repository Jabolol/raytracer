#include "materials/Lambertian.hpp"
#include "core/Payload.hpp"
#include "textures/SolidColor.hpp"

/**
 * @brief Construct a new Lambertian object.
 *
 * This function constructs a new Lambertian object with the given albedo. The
 * Lambertian material scatters light with the given albedo.
 *
 * @param albedo The albedo of the Lambertian material.
 *
 * @return A new Lambertian object.
 */
Raytracer::Materials::Lambertian::Lambertian(const Utils::Color &albedo)
    : _texture(std::make_shared<Textures::SolidColor>(albedo))
{
}

/**
 * @brief Construct a new Lambertian object.
 *
 * This function constructs a new Lambertian object with the given texture. The
 * Lambertian material scatters light with the given texture.
 *
 * @param texture The texture of the Lambertian material.
 *
 * @return A new Lambertian object.
 */
Raytracer::Materials::Lambertian::Lambertian(
    std::shared_ptr<Interfaces::ITexture> texture)
    : _texture(texture)
{
}

/**
 * @brief Scatter the ray with the Lambertian material.
 *
 * This function scatters the ray with the Lambertian material. The function
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
bool Raytracer::Materials::Lambertian::scatter(const Core::Ray &ray,
    const Core::Payload &payload, Utils::Color &attenuation,
    Core::Ray &scattered) const
{
    Utils::Vec3 scatterDirection =
        payload.normal() + Utils::randomUnitVector<double, 3>();

    if (scatterDirection.nearZero()) {
        scatterDirection = payload.normal();
    }

    scattered = Core::Ray(payload.point(), scatterDirection, ray.time());
    attenuation = _texture->value(payload.u(), payload.v(), payload.point());
    return true;
}

/**
 * @brief Emitted light of the Lambertian material.
 *
 * This function returns the emitted light of the Lambertian material. The
 * function returns the emitted light of the material at the given point.
 *
 * @param u The u coordinate of the texture.
 * @param v The v coordinate of the texture.
 * @param point The point to get the emitted light from.
 *
 * @return The emitted light of the material.
 */
Raytracer::Utils::Color Raytracer::Materials::Lambertian::emitted(
    double u, double v, const Utils::Point3 &point) const
{
    return Utils::Color(0.0, 0.0, 0.0);
}
