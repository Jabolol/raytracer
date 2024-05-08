#include "effects/Smoke.hpp"
#include "materials/Isotropic.hpp"

/**
 * @brief Construct a new Smoke object.
 *
 * This function constructs a new Smoke object with the given boundary,
 * density, and texture. The smoke is created within the boundary with the
 * given density and texture. The phase function of the smoke is set to
 * isotropic with the given texture.
 *
 * @param boundary The boundary to create the smoke within.
 * @param density The density of the smoke.
 * @param texture The texture of the smoke.
 *
 * @return A new Smoke object.
 */
Raytracer::Effects::Smoke::Smoke(
    std::shared_ptr<Interfaces::IHittable> boundary, double density,
    std::shared_ptr<Interfaces::ITexture> texture)
    : _boundary(boundary),
      _phaseFunction(std::make_shared<Materials::Isotropic>(texture)),
      _density(-1 / density)
{
}

/**
 * @brief Construct a new Smoke object.
 *
 * This function constructs a new Smoke object with the given boundary,
 * density, and albedo. The smoke is created within the boundary with the
 * given density and albedo. The phase function of the smoke is set to
 * isotropic with the given albedo.
 *
 * @param boundary The boundary to create the smoke within.
 * @param density The density of the smoke.
 * @param albedo The albedo of the smoke.
 *
 * @return A new Smoke object.
 */
Raytracer::Effects::Smoke::Smoke(
    std::shared_ptr<Interfaces::IHittable> boundary, double density,
    const Utils::Color &albedo)
    : _boundary(boundary),
      _phaseFunction(std::make_shared<Materials::Isotropic>(albedo)),
      _density(-1 / density)
{
}

/**
 * @brief Check if the ray hits the smoke.
 *
 * This function checks if the ray hits the smoke. The function returns true if
 * the ray hits the smoke. The function returns false if the ray does not hit
 * the smoke. The function updates the payload with the hit information.
 *
 * @param ray The ray to check for hits.
 * @param interval The interval to check for hits.
 * @param payload The payload to update with the hit information.
 *
 * @return True if the ray hits the smoke, false otherwise.
 */
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

/**
 * @brief Get the bounding box of the smoke.
 *
 * This function returns the bounding box of the smoke.
 *
 * @return The bounding box of the smoke.
 */
Raytracer::Utils::AxisAlignedBBox
Raytracer::Effects::Smoke::boundingBox() const
{
    return _boundary->boundingBox();
}
