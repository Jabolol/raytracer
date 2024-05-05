#include "materials/Dielectric.hpp"
#include "core/Payload.hpp"
#include "utils/Color.hpp"
#include "utils/VecN.hpp"

Raytracer::Materials::Dielectric::Dielectric(double refractionIndex)
    : _refractionIndex(refractionIndex)
{
}

Raytracer::Materials::Dielectric::Dielectric(
    double refractionIndex, const Utils::Color &albedo)
    : _refractionIndex(refractionIndex), _albedo(albedo)
{
}

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

double Raytracer::Materials::Dielectric::reflectance(
    double cosine, double index)
{
    double r0 = (1 - index) / (1 + index);
    r0 = std::pow(r0, 2);

    return r0 + (1 - r0) * std::pow((1 - cosine), 5);
}

Raytracer::Utils::Color Raytracer::Materials::Dielectric::emitted(
    double u, double v, const Utils::Point3 &point) const
{
    return Utils::Color(0.0, 0.0, 0.0);
}
