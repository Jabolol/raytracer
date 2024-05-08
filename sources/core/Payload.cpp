#include "core/Payload.hpp"

/**
 * @brief Set the face normal based on the ray and the outward normal.
 *
 * This function sets the face normal based on the ray and the outward normal.
 * If the ray is outside the object, the face normal is the outward normal. If
 * the ray is inside the object, the face normal is the negative of the outward
 * normal.
 *
 * @param ray The ray to set the face normal from.
 * @param outwardNormal The outward normal to set the face normal from.
 *
 * @return void
 */
void Raytracer::Core::Payload::setFaceNormal(
    const Core::Ray &ray, const Utils::Vec3 &outwardNormal)
{
    _frontFace = Utils::dot(ray.direction(), outwardNormal) < 0;
    _normal = _frontFace ? outwardNormal : -outwardNormal;
}
