#include "textures/Noise.hpp"

/**
 * @brief Construct a new Noise object.
 *
 * This function constructs a new Noise object with the given scale.
 * The Noise texture is a texture that generates Perlin noise.
 *
 * @param scale The scale of the noise texture.
 *
 * @return A new Noise object.
 */
Raytracer::Textures::Noise::Noise(double scale) : _scale(scale), _perlin()
{
}

/**
 * @brief Get the value of the noise texture.
 *
 * This function returns the value of the noise texture at the given UV
 * coordinates and point.
 *
 * @param u The U coordinate.
 * @param v The V coordinate.
 * @param point The point.
 *
 * @return The value of the noise texture.
 */
Raytracer::Utils::Color Raytracer::Textures::Noise::value(
    double u, double v, const Raytracer::Utils::Point3 &point) const
{
    return Utils::Color(0.5, 0.5, 0.5)
        * (1
            + std::sin(
                _scale * point.z() + 10 * _perlin.turbulence(point, 7)));
}
