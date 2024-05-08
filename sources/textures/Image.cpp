#include "textures/Image.hpp"
#include "utils/Color.hpp"
#include "utils/Interval.hpp"

/**
 * @brief Construct a new Image object.
 *
 * This function constructs a new Image object with the given filename.
 *
 * @param filename The filename of the image.
 *
 * @return A new Image object.
 */
Raytracer::Textures::Image::Image(std::string filename)
    : _helper(filename.c_str())
{
}

/**
 * @brief Get the value of the image texture.
 *
 * This function returns the value of the image texture at the given UV
 * coordinates and point.
 *
 * @param u The U coordinate.
 * @param v The V coordinate.
 * @param point The point.
 *
 * @return The value of the image texture.
 */
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
