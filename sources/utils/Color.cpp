#include "utils/Color.hpp"
#include "utils/Interval.hpp"

/**
 * @brief Transform a linear color to a gamma color.
 *
 * This function transforms a linear color to a gamma color.
 *
 * @param linear The linear color.
 *
 * @return The gamma color.
 */
double Raytracer::Utils::linearToGamma(double linear)
{
    if (linear > 0) {
        return std::sqrt(linear);
    }

    return 0;
}

/**
 * @brief Write a color to an output stream.
 *
 * This function writes a color to an output stream.
 *
 * @param out The output stream.
 * @param pixelColor The color.
 */
void Raytracer::Utils::writeColor(std::ostream &out, const Color &pixelColor)
{
    double r = pixelColor.x();
    double g = pixelColor.y();
    double b = pixelColor.z();

    r = linearToGamma(r);
    g = linearToGamma(g);
    b = linearToGamma(b);

    static const Interval intensity(0.000, 0.999);

    int rb = int(256 * intensity.clamp(r));
    int gb = int(256 * intensity.clamp(g));
    int ib = int(256 * intensity.clamp(b));

    out << rb << ' ' << gb << ' ' << ib << '\n';
}
