#include "core/Payload.hpp"

void Raytracer::Core::Payload::setFaceNormal(
    const Core::Ray &ray, const Utils::Vec3 &outwardNormal)
{
    _frontFace = Utils::dot(ray.direction(), outwardNormal) < 0;
    _normal = _frontFace ? outwardNormal : -outwardNormal;
}
