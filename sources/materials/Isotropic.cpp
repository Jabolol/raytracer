#include "materials/Isotropic.hpp"
#include "core/Payload.hpp"
#include "textures/SolidColor.hpp"

Raytracer::Materials::Isotropic::Isotropic(
    std::shared_ptr<Interfaces::ITexture> texture)
    : _texture(texture)
{
}

Raytracer::Materials::Isotropic::Isotropic(const Utils::Color &color)
    : _texture(std::make_shared<Textures::SolidColor>(color))
{
}

bool Raytracer::Materials::Isotropic::scatter(const Core::Ray &ray,
    const Core::Payload &payload, Utils::Color &attenuation,
    Core::Ray &scattered) const
{
    scattered = Core::Ray(
        payload.point(), Utils::randomUnitVector<double, 3>(), ray.time());
    attenuation = _texture->value(payload.u(), payload.v(), payload.point());
    return true;
}

Raytracer::Utils::Color Raytracer::Materials::Isotropic::emitted(
    double u, double v, const Utils::Point3 &point) const
{
    return Utils::Color(0.0, 0.0, 0.0);
}
