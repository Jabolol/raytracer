#include "core/Scene.hpp"

Raytracer::Core::Scene::Scene(std::shared_ptr<Interfaces::IHittable> object)
{
    add(object);
}

void Raytracer::Core::Scene::clear()
{
    _objects.clear();
}

void Raytracer::Core::Scene::add(std::shared_ptr<Interfaces::IHittable> object)
{
    _objects.push_back(object);
    _bbox = Utils::AxisAlignedBBox(_bbox, object->boundingBox());
}

bool Raytracer::Core::Scene::hit(
    const Ray &ray, Utils::Interval interval, Payload &payload) const
{
    Payload temp;
    bool hitAnything = false;
    double closest = interval.max();

    for (const std::shared_ptr<Raytracer::Interfaces::IHittable> &object :
        _objects) {
        if (object->hit(ray, Utils::Interval(interval.min(), closest), temp)) {
            hitAnything = true;
            closest = temp.t();
            payload = temp;
        }
    }

    return hitAnything;
}

Raytracer::Utils::AxisAlignedBBox Raytracer::Core::Scene::boundingBox() const
{
    return _bbox;
}
