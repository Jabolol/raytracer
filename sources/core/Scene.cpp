#include "core/Scene.hpp"

/**
 * @brief Construct a new Scene object.
 *
 * This function constructs a new Scene object with the given object.
 * The object is added to the list of objects in the scene.
 * The bounding box of the scene is updated with the bounding box of the
 * object.
 *
 * @param object The object to add to the scene.
 *
 * @return A new Scene object.
 */
Raytracer::Core::Scene::Scene(std::shared_ptr<Interfaces::IHittable> object)
{
    add(object);
}

/**
 * @brief Clear the scene.
 *
 * This function clears the list of objects in the scene.
 * The bounding box of the scene is reset to the default value.
 * The scene is empty after this function is called.
 *
 * @return void
 */
void Raytracer::Core::Scene::clear()
{
    _objects.clear();
}

/**
 * @brief Add an object to the scene.
 *
 * This function adds an object to the list of objects in the scene.
 * The bounding box of the scene is updated with the bounding box of the
 * object.
 *
 * @param object The object to add to the scene.
 *
 * @return void
 */
void Raytracer::Core::Scene::add(std::shared_ptr<Interfaces::IHittable> object)
{
    _objects.push_back(object);
    _bbox = Utils::AxisAlignedBBox(_bbox, object->boundingBox());
}

/**
 * @brief Check if the ray hits anything in the scene.
 *
 * This function checks if the ray hits anything in the scene.
 * The function returns true if the ray hits anything in the scene.
 * The function returns false if the ray does not hit anything in the scene.
 * The function updates the payload with the hit information.
 *
 * @param ray The ray to check for hits.
 * @param interval The interval to check for hits.
 * @param payload The payload to update with the hit information.
 *
 * @return true if the ray hits anything in the scene, false otherwise.
 */
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

/**
 * @brief Get the bounding box of the scene.
 *
 * This function returns the bounding box of the scene.
 *
 * @return The bounding box of the scene.
 */
Raytracer::Utils::AxisAlignedBBox Raytracer::Core::Scene::boundingBox() const
{
    return _bbox;
}
