#include "textures/Image.hpp"
#include "utils/Color.hpp"
#include "utils/Interval.hpp"

Raytracer::Textures::Image::Image(std::string filename)
    : _helper(filename.c_str())
{
}

Raytracer::Utils::Color Raytracer::Textures::Image::value(
    double u, double v, const Utils::Point3 &point) const
{
    if (_helper.height() <= 0) {
        return Utils::Color(0, 1, 1);
    }

    u = Utils::Interval(0, 1).clamp(u);
    v = 1.0 - Utils::Interval(0, 1).clamp(v);

    int i = static_cast<int>(u * _helper.width());
    int j = static_cast<int>(v * _helper.height());
    const unsigned char *pixel = _helper.pixelData(i, j);

    double colorScale = 1.0 / 255.0;

    return Utils::Color(
        colorScale * pixel[0], colorScale * pixel[1], colorScale * pixel[2]);
}
