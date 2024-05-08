#include "textures/SolidColor.hpp"
#include "utils/Color.hpp"

/**
 * @brief Construct a new SolidColor object.
 *
 * This function constructs a new SolidColor object with the given albedo.
 * The SolidColor texture is a texture that generates a solid color.
 *
 * @param albedo The albedo of the solid color texture.
 *
 * @return A new SolidColor object.
 */
Raytracer::Textures::SolidColor::SolidColor(const Utils::Color &albedo)
    : _albedo(albedo)
{
}

/**
 * @brief Construct a new SolidColor object.
 *
 * This function constructs a new SolidColor object with the given red, green,
 * and blue values.
 * The SolidColor texture is a texture that generates a solid color.
 *
 * @param red The red value of the solid color texture.
 * @param green The green value of the solid color texture.
 * @param blue The blue value of the solid color texture.
 *
 * @return A new SolidColor object.
 */
Raytracer::Textures::SolidColor::SolidColor(
    double red, double green, double blue)
    : _albedo(red, green, blue)
{
}

/**
 * @brief Get the value of the solid color texture.
 *
 * This function returns the value of the solid color texture at the given UV
 * coordinates and point.
 *
 * @param u The U coordinate.
 * @param v The V coordinate.
 * @param point The point.
 *
 * @return The value of the solid color texture.
 */
Raytracer::Utils::Color Raytracer::Textures::SolidColor::value(
    double u, double v, const Utils::Point3 &point) const
{
    return _albedo;
}
