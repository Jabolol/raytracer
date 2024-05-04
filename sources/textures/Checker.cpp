#include "textures/Checker.hpp"
#include "textures/SolidColor.hpp"

Raytracer::Textures::Checker::Checker(double scale,
    std::shared_ptr<Interfaces::ITexture> even,
    std::shared_ptr<Interfaces::ITexture> odd)
    : _odd(odd), _even(even), _scale(1.0 / scale)
{
}

Raytracer::Textures::Checker::Checker(
    double scale, const Utils::Color &a, const Utils::Color &b)
    : _odd(std::make_shared<Textures::SolidColor>(b)),
      _even(std::make_shared<Textures::SolidColor>(a)), _scale(1.0 / scale)
{
}

Raytracer::Utils::Color Raytracer::Textures::Checker::value(
    double u, double v, const Utils::Point3 &point) const
{
    int x = static_cast<int>(std::floor(_scale * point.x()));
    int y = static_cast<int>(std::floor(_scale * point.y()));
    int z = static_cast<int>(std::floor(_scale * point.z()));

    bool isEven = (x + y + z) % 2 == 0;

    return isEven ? _even->value(u, v, point) : _odd->value(u, v, point);
}
