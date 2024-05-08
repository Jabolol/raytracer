#include "materials/DiffuseLight.hpp"
#include "textures/SolidColor.hpp"

/**
 * @brief Construct a new DiffuseLight object.
 *
 * This function constructs a new DiffuseLight object with the given texture.
 * The diffuse light material emits light with the given texture.
 *
 * @param texture The texture of the diffuse light.
 *
 * @return A new DiffuseLight object.
 */
Raytracer::Materials::DiffuseLight::DiffuseLight(
    std::shared_ptr<Interfaces::ITexture> texture)
    : _texture(texture)
{
}

/**
 * @brief Construct a new DiffuseLight object.
 *
 * This function constructs a new DiffuseLight object with the given color. The
 * diffuse light material emits light with the given color.
 *
 * @param color The color of the diffuse light.
 *
 * @return A new DiffuseLight object.
 */
Raytracer::Materials::DiffuseLight::DiffuseLight(const Utils::Color &color)
    : _texture(std::make_shared<Textures::SolidColor>(color))
{
}

/**
 * @brief Scatter the ray with the diffuse light material.
 *
 * This function scatters the ray with the diffuse light material. The function
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
bool Raytracer::Materials::DiffuseLight::scatter(const Core::Ray &ray,
    const Core::Payload &payload, Utils::Color &attenuation,
    Core::Ray &scattered) const
{
    return false;
}

/**
 * @brief Emitted light of the diffuse light material.
 *
 * This function returns the emitted light of the diffuse light material. The
 * function returns the emitted light of the material at the given point.
 *
 * @param u The u coordinate of the texture.
 * @param v The v coordinate of the texture.
 * @param point The point to get the emitted light from.
 *
 * @return The emitted light of the material.
 */
Raytracer::Utils::Color Raytracer::Materials::DiffuseLight::emitted(
    double u, double v, const Utils::Point3 &point) const
{
    return _texture->value(u, v, point);
}
