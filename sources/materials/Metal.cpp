#include "materials/Metal.hpp"
#include "core/Payload.hpp"

Raytracer::Materials::Metal::Metal(const Utils::Color &albedo, double fuzz)
    : _albedo(albedo), _fuzz(fuzz)
{
}

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

Raytracer::Utils::Color Raytracer::Materials::Metal::emitted(
    double u, double v, const Utils::Point3 &point) const
{
    return Utils::Color(0.0, 0.0, 0.0);
}
