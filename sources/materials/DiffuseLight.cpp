#include "materials/DiffuseLight.hpp"
#include "textures/SolidColor.hpp"

Raytracer::Materials::DiffuseLight::DiffuseLight(
    std::shared_ptr<Interfaces::ITexture> texture)
    : _texture(texture)
{
}

Raytracer::Materials::DiffuseLight::DiffuseLight(const Utils::Color &color)
    : _texture(std::make_shared<Textures::SolidColor>(color))
{
}

bool Raytracer::Materials::DiffuseLight::scatter(const Core::Ray &ray,
    const Core::Payload &payload, Utils::Color &attenuation,
    Core::Ray &scattered) const
{
    return false;
}

Raytracer::Utils::Color Raytracer::Materials::DiffuseLight::emitted(
    double u, double v, const Utils::Point3 &point) const
{
    return _texture->value(u, v, point);
}
