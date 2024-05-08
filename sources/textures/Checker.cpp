#include "textures/Checker.hpp"
#include "textures/SolidColor.hpp"

/**
 * @brief Construct a new Checker object.
 *
 * This function constructs a new Checker object with the given scale, even
 * texture, and odd texture.
 *
 * @param scale The scale of the checker texture.
 * @param even The even texture of the checker texture.
 * @param odd The odd texture of the checker texture.
 *
 * @return A new Checker object.
 */
Raytracer::Textures::Checker::Checker(double scale,
    std::shared_ptr<Interfaces::ITexture> even,
    std::shared_ptr<Interfaces::ITexture> odd)
    : _odd(odd), _even(even), _scale(1.0 / scale)
{
}

/**
 * @brief Construct a new Checker object.
 *
 * This function constructs a new Checker object with the given scale, even
 * color, and odd color.
 *
 * @param scale The scale of the checker texture.
 * @param a The even color of the checker texture.
 * @param b The odd color of the checker texture.
 *
 * @return A new Checker object.
 */
Raytracer::Textures::Checker::Checker(
    double scale, const Utils::Color &a, const Utils::Color &b)
    : _odd(std::make_shared<Textures::SolidColor>(b)),
      _even(std::make_shared<Textures::SolidColor>(a)), _scale(1.0 / scale)
{
}

/**
 * @brief Get the value of the checker texture.
 *
 * This function returns the value of the checker texture at the given UV
 * coordinates and point.
 *
 * @param u The U coordinate.
 * @param v The V coordinate.
 * @param point The point.
 *
 * @return The value of the checker texture.
 */
Raytracer::Utils::Color Raytracer::Textures::Checker::value(
    double u, double v, const Utils::Point3 &point) const
{
    int x = static_cast<int>(std::floor(_scale * point.x()));
    int y = static_cast<int>(std::floor(_scale * point.y()));
    int z = static_cast<int>(std::floor(_scale * point.z()));

    bool isEven = (x + y + z) % 2 == 0;

    return isEven ? _even->value(u, v, point) : _odd->value(u, v, point);
}
