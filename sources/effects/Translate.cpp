#include "effects/Translate.hpp"

Raytracer::Effects::Translate::Translate(
    std::shared_ptr<Interfaces::IHittable> object, const Utils::Vec3 &offset)
    : _object(object), _offset(offset)
{
    _bbox = object->boundingBox() + offset;
}

bool Raytracer::Effects::Translate::hit(const Core::Ray &ray,
    Utils::Interval interval, Core::Payload &payload) const
{
    Core::Ray offsetRay(ray.origin() - _offset, ray.direction(), ray.time());

    if (!_object->hit(offsetRay, interval, payload)) {
        return false;
    }

    Utils::Point3 point = payload.point();
    payload.point(point + _offset);

    return true;
}

Raytracer::Utils::AxisAlignedBBox
Raytracer::Effects::Translate::boundingBox() const
{
    return _bbox;
}
