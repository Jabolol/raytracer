#include "effects/Smoke.hpp"
#include "materials/Isotropic.hpp"

Raytracer::Effects::Smoke::Smoke(
    std::shared_ptr<Interfaces::IHittable> boundary, double density,
    std::shared_ptr<Interfaces::ITexture> texture)
    : _boundary(boundary),
      _phaseFunction(std::make_shared<Materials::Isotropic>(texture)),
      _density(-1 / density)
{
}

Raytracer::Effects::Smoke::Smoke(
    std::shared_ptr<Interfaces::IHittable> boundary, double density,
    const Utils::Color &albedo)
    : _boundary(boundary),
      _phaseFunction(std::make_shared<Materials::Isotropic>(albedo)),
      _density(-1 / density)
{
}

bool Raytracer::Effects::Smoke::hit(const Core::Ray &ray,
    Utils::Interval interval, Core::Payload &payload) const
{
    double infinity = std::numeric_limits<double>::infinity();
    Core::Payload payloadOne, payloadTwo;

    if (!_boundary->hit(ray, Utils::Interval::Universe, payloadOne)) {
        return false;
    }

    if (!_boundary->hit(ray,
            Utils::Interval(payloadOne.t() + 0.0001, infinity), payloadTwo)) {
        return false;
    }

    if (payloadOne.t() < interval.min())
        payloadOne.t(interval.min());
    if (payloadTwo.t() > interval.max())
        payloadTwo.t(interval.max());

    if (payloadOne.t() >= payloadTwo.t()) {
        return false;
    }

    if (payloadOne.t() < 0) {
        payloadOne.t(0);
    }

    double length = ray.direction().length();
    double boundaryDistance = (payloadTwo.t() - payloadOne.t()) * length;
    double hitDistance = _density * log(Utils::randomDouble());

    if (hitDistance > boundaryDistance) {
        return false;
    }

    payload.t(payloadOne.t() + hitDistance / length);
    payload.point(ray.at(payload.t()));

    payload.normal(Utils::Vec3(1, 0, 0));
    payload.frontFace(true);
    payload.material(_phaseFunction);

    return true;
}

Raytracer::Utils::AxisAlignedBBox
Raytracer::Effects::Smoke::boundingBox() const
{
    return _boundary->boundingBox();
}
