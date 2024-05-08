#include "materials/Isotropic.hpp"
#include "core/Payload.hpp"
#include "textures/SolidColor.hpp"

/**
 * @brief Construct a new Isotropic object.
 *
 * This function constructs a new Isotropic object with the given texture.
 *
 * @param texture The texture of the isotropic material.
 *
 * @return A new Isotropic object.
 */
Raytracer::Materials::Isotropic::Isotropic(
    std::shared_ptr<Interfaces::ITexture> texture)
    : _texture(texture)
{
}

/**
 * @brief Construct a new Isotropic object.
 *
 * This function constructs a new Isotropic object with the given color.
 *
 * @param color The color of the isotropic material.
 *
 * @return A new Isotropic object.
 */
Raytracer::Materials::Isotropic::Isotropic(const Utils::Color &color)
    : _texture(std::make_shared<Textures::SolidColor>(color))
{
}

/**
 * @brief Scatter the ray with the isotropic material.
 *
 * This function scatters the ray with the isotropic material. The function
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
bool Raytracer::Materials::Isotropic::scatter(const Core::Ray &ray,
    const Core::Payload &payload, Utils::Color &attenuation,
    Core::Ray &scattered) const
{
    scattered = Core::Ray(
        payload.point(), Utils::randomUnitVector<double, 3>(), ray.time());
    attenuation = _texture->value(payload.u(), payload.v(), payload.point());
    return true;
}

/**
 * @brief Emitted light of the isotropic material.
 *
 * This function returns the emitted light of the isotropic material. The
 * function returns the emitted light of the material at the given point.
 *
 * @param u The u texture coordinate.
 * @param v The v texture coordinate.
 * @param point The point to get the emitted light from.
 *
 * @return The emitted light of the isotropic material.
 */
Raytracer::Utils::Color Raytracer::Materials::Isotropic::emitted(
    double u, double v, const Utils::Point3 &point) const
{
    return Utils::Color(0.0, 0.0, 0.0);
}
