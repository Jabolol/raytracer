#include "textures/Noise.hpp"

Raytracer::Textures::Noise::Noise(double scale) : _scale(scale), _perlin()
{
}

Raytracer::Utils::Color Raytracer::Textures::Noise::value(
    double u, double v, const Raytracer::Utils::Point3 &point) const
{
    return Utils::Color(0.5, 0.5, 0.5)
        * (1
            + std::sin(
                _scale * point.z() + 10 * _perlin.turbulence(point, 7)));
}
