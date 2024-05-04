#include "textures/SolidColor.hpp"
#include "utils/Color.hpp"

Raytracer::Textures::SolidColor::SolidColor(const Utils::Color &albedo)
    : _albedo(albedo)
{
}

Raytracer::Textures::SolidColor::SolidColor(
    double red, double green, double blue)
    : _albedo(red, green, blue)
{
}

Raytracer::Utils::Color Raytracer::Textures::SolidColor::value(
    double u, double v, const Utils::Point3 &point) const
{
    return _albedo;
}
