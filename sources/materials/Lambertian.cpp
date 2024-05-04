#include "materials/Lambertian.hpp"
#include "core/Payload.hpp"
#include "textures/SolidColor.hpp"

Raytracer::Materials::Lambertian::Lambertian(const Utils::Color &albedo)
    : _texture(std::make_shared<Textures::SolidColor>(albedo))
{
}

Raytracer::Materials::Lambertian::Lambertian(
    std::shared_ptr<Interfaces::ITexture> texture)
    : _texture(texture)
{
}

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

Raytracer::Utils::Color Raytracer::Materials::Lambertian::emitted(
    double u, double v, const Utils::Point3 &point) const
{
    return Utils::Color(0.0, 0.0, 0.0);
}
