#include "utils/Color.hpp"
#include "utils/Interval.hpp"

double Raytracer::Utils::linearToGamma(double linear)
{
    if (linear > 0) {
        return std::sqrt(linear);
    }

    return 0;
}

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
